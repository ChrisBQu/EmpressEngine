#ifndef ENGINE_H
#define ENGINE_H

#include "Controller.h"
#include "GL_Renderer.h"

#include <SDL.h>
#include <GL/glew.h>

enum class EngineErrorCode { SUCCESS, SDL_UNINITIATED, NULL_WINDOW, NULL_GL_RENDERER, NULL_SDL_RENDERER, GL_CONTEXT_NULL, GLEW_INIT_FAILURE, GL_INIT_FAILURE, TTF_ERROR};

class Engine {
public:
	Engine();
	EngineErrorCode init(const char *windowLabel, unsigned int width, unsigned int height, bool fullscreen, int fps);
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
	GL_Renderer myGLRenderer;
	SDL_Renderer *mySDLRenderer;
};

#endif // ENGINE_H