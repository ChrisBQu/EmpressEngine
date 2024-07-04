
#include "Engine.h"
#include "GameConfig.h"
#include "Logger.h"

#include "Geometry.h"

int main(int argc, char* args[])
{

	GeometryTriangle tsmall = { { -1, -1 }, {5, 5}, {1, -1} };

	GeometryRectangle rbig = { {0, 0}, {100, 100} };

	std::vector<GeometryPoint> coll = geometryGetIntersections(rbig, tsmall);
	LOG(coll.size());

	Engine engine;
	engine.init(WINDOW_LABEL, gameConfig.screenWidth, gameConfig.screenHeight, false, gameConfig.FPS);
	engine.run();

	return 0;
}
