
#include "Engine.h"
#include "GameConfig.h"
#include "Logger.h"
#include "GL_Renderer.h"

#include <iostream>

#include "Geometry.h"

int main(int argc, char* args[])
{
	Engine engine;
	engine.init(WINDOW_LABEL, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, false, TARGET_FPS);
	engine.run();

	return 0;
}
