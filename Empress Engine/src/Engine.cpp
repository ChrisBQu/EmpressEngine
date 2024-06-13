#include "Engine.h"
#include "GameConfig.h"
#include "GameObject.h"
#include "GL_Renderer.h"
#include "Logger.h"
#include "RenderInterface.h"
#include "SpriteAnimation.h"

#include <iostream>


GameObject myObj;
GameObject myObj2;

Engine::Engine() {
    frameCount = 0;
    frameStartTime = 0;
    myWindow = NULL;
    myGLRenderer = GL_Renderer();
    targetFPS = 60;
    active = false;
}

std::vector<unsigned int> someTiles;
std::vector<unsigned int> someTiles2;
std::vector<unsigned int> someTiles3;
std::vector<unsigned int> someTiles4;


int Engine::init(const char* label, unsigned int width, unsigned int height, bool fullscreen, int fps) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
        LOG_ERROR("Failed to initialize SDL: \n", SDL_GetError());
        return ERROR_CODE_SDL_UNINITIATED;
    }

    for (int i = 0; i < 20000; i++) {
        someTiles.push_back(2);
        someTiles2.push_back(2);
        someTiles3.push_back(2);
        someTiles4.push_back(2);

    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    int window_flags = SDL_WINDOW_OPENGL | (fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN);
    myWindow = SDL_CreateWindow(label, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);
    if (myWindow == NULL) {
        LOG_ERROR("Failed to create window: \n", SDL_GetError());
        return ERROR_CODE_NULL_WINDOW;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(myWindow);
    if (!glContext) {
        LOG_ERROR("Failed to create OpenGL context: \n", SDL_GetError());
        return ERROR_CODE_GL_CONTEXT_NULL;
    }

    SDL_GL_MakeCurrent(myWindow, glContext);
    SDL_GL_SetSwapInterval(1); // Enable V-Sync

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        LOG_ERROR("Failed to initialize GLEW: \n", glewGetErrorString(err));
        return ERROR_CODE_GLEW_INIT_FAILURE;
    }

    while ((err = glGetError()) != GL_NO_ERROR) {
        LOG_ERROR("OpenGL error during initialization: \n", err);
        return ERROR_CODE_GL_INIT_FAILURE;
    }

    int renderer_status = myGLRenderer.init();
    if (renderer_status != 0) {
        LOG_ERROR("Error initializing renderer - Error code: ", renderer_status);
        return ERROR_CODE_NULL_RENDERER;
    }
    myController.findController();
    myController.initDefaultKeyBindings();

    renderData.gameCamera.pos = { 0, 0 };
    renderData.gameCamera.dimensions = { 320, 240 };

    renderData.uiCamera.pos = { 0, 0 };
    renderData.uiCamera.dimensions = { 1280, 800 };

    active = true;

    buildSpriteAnimationList(ANIMATION_MANIFEST_FILEPATH);

    myObj.sprite.animation = getSpriteAnimationData("PINK_BLOB_STAND");

    return 0;
}

float xxx = 0;
float yyy = 0;
float rrr = 0;

void Engine::handleInput() {

    myController.resetInput();

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) { active = false; }

        if (event.type == SDL_KEYUP) {
            if (event.key.keysym.sym == SDLK_ESCAPE) { active = false; }
        }

        if (event.type == SDL_KEYUP || event.type == SDL_KEYDOWN ||
            event.type == SDL_CONTROLLERBUTTONUP || event.type == SDL_CONTROLLERBUTTONDOWN ||
            event.type == SDL_CONTROLLERAXISMOTION) {
            myController.handleInput(event);
        }


        if (event.type == SDL_CONTROLLERDEVICEADDED || event.type == SDL_CONTROLLERDEVICEREMOVED) {
            myController.handleDeviceConnections(event);
        }
    }


    if (myController.getButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
        renderData.gameCamera.pos.x += 2;
        //xxx += 2;
    }
    if (myController.getButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
        renderData.gameCamera.pos.x -= 2;

        //xxx -= 2;
    }
    if (myController.getButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP)) {
        renderData.gameCamera.pos.y -= 2;
    }
    if (myController.getButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
        renderData.gameCamera.pos.y += 2;
    }

    if (myController.getPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
        myObj.sprite.animation = getSpriteAnimationData("PINK_BLOB_WALK");
        myObj.transform.scale = { 1.0, 1.0 };

    }

    if (myController.getReleased(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
        myObj.sprite.animation = getSpriteAnimationData("PINK_BLOB_STAND");
        myObj.transform.scale = { 1.0, 1.0 };

    }

    if (myController.getPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
        myObj.sprite.animation = getSpriteAnimationData("PINK_BLOB_WALK");
        myObj.transform.scale = { -1.0, 1.0 };

    }

    if (myController.getReleased(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
        myObj.sprite.animation = getSpriteAnimationData("PINK_BLOB_STAND");
        myObj.transform.scale = { -1.0, 1.0 };

    }

}


void Engine::update() {

    myObj.transform.pos = { xxx, yyy };
    myObj.update();

    frameCount++;

}

TilesetData tsd = {
    "tex2", {16, 16}, 2, 16, 
    {
        {0, {0, 1}},
        {1, {1, 1}},
        {2, {3, 12}},
        {3, {2, 12}}
    },
};


void Engine::render() {

    drawTileset(tsd, someTiles, 40, frameCount, 51);
    drawTileset(tsd, someTiles2, 40, frameCount, 51);


    myObj.transform.depth = 50;
    myObj2.transform.depth = 52;
    myObj.render();
    myObj2.render();


    myGLRenderer.render();

    SDL_GL_SwapWindow(myWindow);
}

void Engine::run() {
    active = true;
    Uint32 fpsStartTime = SDL_GetTicks();

    while (active) {
        Uint32 frameStartTime = SDL_GetTicks();

        handleInput();
        update();
        render();

        Uint32 frameTime = SDL_GetTicks() - frameStartTime;
        const int frameDelay = 1000 / targetFPS;
        if (frameTime < frameDelay) {
            SDL_Delay(frameDelay - frameTime);
        }

        Uint32 elapsedTime = SDL_GetTicks() - fpsStartTime;
        if (elapsedTime >= 1000) { // Every second
            float fps = frameCount / (elapsedTime / 1000.0f);
            std::cout << "FPS: " << fps << std::endl;
            frameCount = 0;
            fpsStartTime = SDL_GetTicks();
        }
    }

    SDL_DestroyWindow(myWindow);
    SDL_Quit();
}

