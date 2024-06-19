#include "Engine.h"
#include "GameConfig.h"
#include "GameObject.h"
#include "GL_Renderer.h"
#include "Logger.h"
#include "RenderInterface.h"
#include "Scene.h"
#include "SpriteAnimation.h"

#include <iostream>

GameObject myObj;
std::vector<unsigned int> someTiles;
std::vector<unsigned int> randX;
std::vector<unsigned int> randY;
TilesetData tsd = {
    "tex2", {16, 16}, 2,
    {
        {0, {0, 1}},
        {1, {1, 1}},
        {2, {3, 12}},
        {3, {2, 12}}
    },
};

int depth;
unsigned int tilesPerRow;
glm::vec2 pos;
std::vector<unsigned int> tileData;
TilesetData tilesetData;
Scene myScene;

Engine::Engine() {
    frameCount = 0;
    frameStartTime = 0;
    myWindow = nullptr;
    myGLRenderer = GL_Renderer();
    active = false;
}

EngineErrorCode Engine::init(const char* label, unsigned int width, unsigned int height, bool fullscreen, int fps) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
        LOG_ERROR("Failed to initialize SDL: \n", SDL_GetError());
        return EngineErrorCode::SDL_UNINITIATED;
    }

    // TESTING
    for (int i = 0; i < 1000000; i++) {
        someTiles.push_back(rand()%4);
    }
    for (int i = 0; i < 500; i++) {
        randX.push_back(rand() % 1280);
        randY.push_back(rand() % 720);
    }
    //

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    int window_flags = SDL_WINDOW_OPENGL | (fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_SHOWN);
    myWindow = SDL_CreateWindow(label, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, window_flags);
    if (myWindow == NULL) {
        LOG_ERROR("Failed to create window: \n", SDL_GetError());
        return EngineErrorCode::NULL_WINDOW;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(myWindow);
    if (!glContext) {
        LOG_ERROR("Failed to create OpenGL context: \n", SDL_GetError());
        return EngineErrorCode::GL_CONTEXT_NULL;
    }

    SDL_GL_MakeCurrent(myWindow, glContext);
    SDL_GL_SetSwapInterval(1); // Enable V-Sync

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        LOG_ERROR("Failed to initialize GLEW: \n", glewGetErrorString(err));
        return EngineErrorCode::GLEW_INIT_FAILURE;
    }

    while ((err = glGetError()) != GL_NO_ERROR) {
        LOG_ERROR("OpenGL error during initialization: \n", err);
        return EngineErrorCode::GL_INIT_FAILURE;
    }

    int renderer_status = myGLRenderer.init();
    if (renderer_status != 0) {
        LOG_ERROR("Error initializing renderer - Error code: ", renderer_status);
        return EngineErrorCode::NULL_GL_RENDERER;
    }

    myController.findController();
    myController.initDefaultKeyBindings();

    renderData.gameCamera.pos = { 0, 0 };
    renderData.gameCamera.dimensions = { 426, 240 };

    active = true;

    buildSpriteAnimationList(ANIMATION_MANIFEST_FILEPATH);

    myObj.sprite.animation = getSpriteAnimationData("PINK_BLOB_STAND");
    myScene.addObject(myObj);
    TilesetLayerData tld = { 1000, 100, {0, 0}, someTiles, tsd };
    myScene.setTileLayer(0, tld);
    myScene.toggleTileLayer(0, true);

    return EngineErrorCode::SUCCESS;
}

float xxx = 160;
float yyy = 200;

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
    }
    if (myController.getButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT)) {
        renderData.gameCamera.pos.x -= 2;
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

    myScene.update();

    frameCount++;

}


void Engine::render() {


    drawText("HELLO!!!", "pixel", { 64, 64 }, { 1, 1 }, { 0,0,255,255 });

    myScene.render();


    myGLRenderer.render();

    SDL_GL_SwapWindow(myWindow);
}

void Engine::run() {
    active = true;
    Uint32 fpsStartTime = SDL_GetTicks();
    int fpsframeCount = 0;

    while (active) {
        Uint32 frameStartTime = SDL_GetTicks();

        handleInput();
        update();
        render();

        Uint32 frameTime = SDL_GetTicks() - frameStartTime;
        const int frameDelay = 1000 / gameConfig.FPS;
        if (frameTime < frameDelay) { SDL_Delay(frameDelay - frameTime); }
        fpsframeCount++;
        Uint32 elapsedTime = SDL_GetTicks() - fpsStartTime;
        if (elapsedTime >= 1000) {
            float fps = fpsframeCount / (elapsedTime / 1000.0f);
            fpsframeCount = 0;
            fpsStartTime = SDL_GetTicks();
            if (PRINT_FPS_TO_CONSOLE) { Logger::log(CONSOLE_COLOR_BLUE, "FPS", fps); }
        }
    }

    SDL_DestroyWindow(myWindow);
    SDL_Quit();
}

