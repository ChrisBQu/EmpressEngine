
#include "Engine.h"
#include "GameConfig.h"
#include "Logger.h"

#include "Geometry.h"

int main(int argc, char* args[])
{

	Engine engine;
	engine.init(WINDOW_LABEL, gameConfig.screenWidth, gameConfig.screenHeight, false, gameConfig.FPS);
	engine.run();

	return 0;
}
