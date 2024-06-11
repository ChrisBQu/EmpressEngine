#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <glm/glm.hpp>

struct GeometryPoint {
	float x;
	float y;
};

struct GeometryLineSegment {
	GeometryPoint start;
	GeometryPoint end;
};

struct GeometryRay {
	GeometryPoint start;
	GeometryPoint direction;
};

struct GeometryCircle {
	GeometryPoint pos;
	float radius;
};

struct GeometryRectangle {
	GeometryPoint pos;
	GeometryPoint size;
};


enum class GeometryType {
	POINT, LINE_SEGMENT, CIRCLE, RECTANGLE, RAY
};

struct GeometryShape {
	GeometryType shapetype; // Should be GEOMETRY_TYPE::POINT, GEOMETRY_TYPE::LINE_SEGMENT, GEOMETRY_TYPE::CIRCLE or GEOMETRY_TYPE::RECTANGLE
	union {
		GeometryPoint point;
		GeometryCircle circle;
		GeometryLineSegment line;
		GeometryRectangle rectangle;
		GeometryRay ray;
	} shape;
};


std::vector<GeometryPoint> geometryGetIntersections(GeometryShape first, GeometryShape second);
bool geometryShapeContains(GeometryShape first, GeometryShape second);

#endif // GEOMETRY_H