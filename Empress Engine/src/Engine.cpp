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
    myRenderer = NULL;
    targetFPS = 60;
    active = false;
}

int Engine::init(const char* label, unsigned int width, unsigned int height, bool fullscreen, int fps) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
        LOG_ERROR("Failed to initialize SDL: \n", SDL_GetError());
        return ERROR_CODE_SDL_UNINITIATED;
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

    initGLRenderer();
    myRenderer = SDL_CreateRenderer(myWindow, -1, 0);
    if (myRenderer == NULL) {
        LOG_ERROR("Failed to create SDL Renderer: \n", SDL_GetError());
        return ERROR_CODE_NULL_RENDERER;
    }
    SDL_SetRenderDrawColor(myRenderer, 255, 255, 255, 255);

    myController.findController();
    myController.initDefaultKeyBindings();

    renderData.gameCamera.pos = { 40, 0 };
    renderData.gameCamera.dimensions = { 640, 480 };

    renderData.uiCamera.pos = { 0, 0 };
    renderData.uiCamera.dimensions = { 1280, 800 };

    active = true;

    buildSpriteAnimationList(ANIMATION_MANIFEST_FILEPATH);

    myObj.sprite.animation = getSpriteAnimationData("PINK_BLOB_STAND");
    myObj2.sprite.animation = getSpriteAnimationData("BLUE_BLOB_WALK");


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
        xxx += 2;
    }
    if (myController.getButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
        xxx -= 2;
    }
    if (myController.getButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP)) {
        yyy -= 2;
    }
    if (myController.getButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN)) {
        yyy += 2;
    }

    if (myController.getPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
        myObj.sprite.animation = getSpriteAnimationData("PINK_BLOB_WALK");
    }

    if (myController.getReleased(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) {
        myObj.sprite.animation = getSpriteAnimationData("PINK_BLOB_STAND");
    }

}


void Engine::update() {

    renderData.gameCamera.pos = { 0, 0 };
    renderData.gameCamera.dimensions = { 640, 480 };


    myObj.transform.pos = { xxx, yyy };
    myObj2.transform.pos = { xxx, yyy+32 };
    myObj.update();
    myObj2.update();
    myObj2.transform.scale = { -1.0, 1.0 };

    frameCount++;

}

void Engine::render() {
    glRender();

    myObj.render();
    myObj2.render();

    SDL_GL_SwapWindow(myWindow);
}

void Engine::run() {
    active = true;
    while (active) {
        frameStartTime = SDL_GetTicks();
        handleInput();
        update();
        render();
        int frameTime = SDL_GetTicks() - frameStartTime;
        const int frameDelay = 1000 / targetFPS;
        if (frameDelay > frameTime) { SDL_Delay(frameDelay - frameTime); }
    }
    SDL_DestroyWindow(myWindow);
    SDL_Quit();
}
