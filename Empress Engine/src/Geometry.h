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

struct GeometryTriangle {
	GeometryPoint a;
	GeometryPoint b;
	GeometryPoint c;
};

enum class GeometryType {
	POINT, LINE_SEGMENT, CIRCLE, RECTANGLE, RAY, TRIANGLE
};

struct GeometryShape {
	GeometryType shapetype;
	union {
		GeometryPoint point;
		GeometryCircle circle;
		GeometryLineSegment line;
		GeometryRectangle rectangle;
		GeometryRay ray;
		GeometryTriangle triangle;
	} shape;
};


std::vector<GeometryPoint> geometryGetIntersections(GeometryShape first, GeometryShape second);
bool geometryShapeContains(GeometryShape first, GeometryShape second);

#endif // GEOMETRY_H