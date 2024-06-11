
#include "Engine.h"
#include "GameConfig.h"
#include "Logger.h"
#include "GL_Renderer.h"

#include <iostream>

#include "Geometry.h"
// Function to print GeometryPoints
void printPoints(const std::vector<GeometryPoint>& points) {
    for (const auto& point : points) {
        std::cout << "Intersection at (" << point.x << ", " << point.y << ")\n";
    }
}

// Function to print test results
void printResult(const std::string& testName, const std::vector<GeometryPoint>& intersections) {
    std::cout << testName << ": ";
    if (intersections.empty()) {
        std::cout << "No intersections\n";
    }
    else {
        printPoints(intersections);
    }
}


int main(int argc, char* args[])
{
    // Test case 1: Ray-ray intersection
    GeometryShape ray1, ray2;
    ray1.shapetype = GeometryType::RAY;
    ray1.shape.ray.start = { 0, 0 };
    ray1.shape.ray.direction = { 1, 1 };
    ray2.shapetype = GeometryType::RAY;
    ray2.shape.ray.start = { 0, 1 };
    ray2.shape.ray.direction = { 1, -1 };

    std::vector<GeometryPoint> intersections = geometryGetIntersections(ray1, ray2);
    printResult("Test case 1: Ray-ray intersection", intersections);

    // Test case 2: Ray-point intersection
    GeometryShape point;
    point.shapetype = GeometryType::POINT;
    point.shape.point = { 2, 2 };

    intersections = geometryGetIntersections(ray1, point);
    printResult("Test case 2: Ray-point intersection", intersections);

    // Test case 3: Ray-line intersection
    GeometryShape line;
    line.shapetype = GeometryType::LINE_SEGMENT;
    line.shape.line.start = { 0, 1 };
    line.shape.line.end = { 4, 1 };

    intersections = geometryGetIntersections(ray1, line);
    printResult("Test case 3: Ray-line intersection", intersections);

    // Test case 4: Ray-circle intersection
    GeometryShape circle;
    circle.shapetype = GeometryType::CIRCLE;
    circle.shape.circle.pos = { 3, 3 };
    circle.shape.circle.radius = 2;

    intersections = geometryGetIntersections(ray1, circle);
    printResult("Test case 4: Ray-circle intersection", intersections);

    // Test case 5: Ray-rectangle intersection
    GeometryShape rect;
    rect.shapetype = GeometryType::RECTANGLE;
    rect.shape.rectangle.pos = { 2, 2 };
    rect.shape.rectangle.size = { 3, 3 };

    intersections = geometryGetIntersections(ray1, rect);
    printResult("Test case 5: Ray-rectangle intersection", intersections);

	Engine engine;
	engine.init(WINDOW_LABEL, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, false, TARGET_FPS);
	engine.run();

	return 0;
}
