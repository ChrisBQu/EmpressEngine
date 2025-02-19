#ifndef ENGINE_H
#define ENGINE_H

#include "Controller.h"
#include "GL_Renderer.h"

#include <SDL.h>
#include <GL/glew.h>

enum class EngineErrorCode { SUCCESS, SDL_UNINITIATED, NULL_WINDOW, NULL_GL_RENDERER, GL_CONTEXT_NULL, GLEW_INIT_FAILURE, GL_INIT_FAILURE, MIXER_ERROR };

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
	bool active;
	Controller myController;
	SDL_Window* myWindow;
	GL_Renderer myGLRenderer;
};

#endif // ENGINE_H