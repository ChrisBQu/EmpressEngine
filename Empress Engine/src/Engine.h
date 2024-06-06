#ifndef ENGINE_H
#define ENGINE_H

#include "Controller.h"
#include <SDL.h>
#include <GL/glew.h>

class Engine {
public:
	Engine();
	int init(const char *windowLabel, unsigned int width, unsigned int height, bool fullscreen, int fps);
	void run();
private:
	void handleInput();
	void update();
	void render();
	Uint32 frameCount;
	Uint32 frameStartTime;
	unsigned int targetFPS;
	bool active;
	Controller myController;
	SDL_Window* myWindow;
	SDL_Renderer* myRenderer;
};

const Uint8 ERROR_CODE_SDL_UNINITIATED = 1;
const Uint8 ERROR_CODE_NULL_WINDOW = 2;
const Uint8 ERROR_CODE_NULL_RENDERER = 3;
const Uint8 ERROR_CODE_GL_CONTEXT_NULL = 4;
const Uint8 ERROR_CODE_GLEW_INIT_FAILURE = 5;
const Uint8 ERROR_CODE_GL_INIT_FAILURE = 6;

#endif // ENGINE_H