#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>

enum class GeometryType {
    POINT, LINE_SEGMENT, CIRCLE, RECTANGLE, RAY, TRIANGLE
};

class GeometryShape {
public:
    virtual ~GeometryShape() = default;
    virtual GeometryType getType() const = 0;
    virtual GeometryShape* clone() const = 0;
};

class GeometryPoint : public GeometryShape {
public:
    float x;
    float y;

    GeometryPoint(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}

    GeometryType getType() const override { return GeometryType::POINT; }
    GeometryShape* clone() const override { return new GeometryPoint(*this); }
};

class GeometryLineSegment : public GeometryShape {
public:
    GeometryPoint start;
    GeometryPoint end;

    GeometryLineSegment(const GeometryPoint& start = GeometryPoint(), const GeometryPoint& end = GeometryPoint()) : start(start), end(end) {}

    GeometryType getType() const override { return GeometryType::LINE_SEGMENT; }
    GeometryShape* clone() const override { return new GeometryLineSegment(*this); }
};

class GeometryRay : public GeometryShape {
public:
    GeometryPoint start;
    GeometryPoint direction;

    GeometryRay(const GeometryPoint& start = GeometryPoint(), const GeometryPoint& direction = GeometryPoint()) : start(start), direction(direction) {}

    GeometryType getType() const override { return GeometryType::RAY; }
    GeometryShape* clone() const override { return new GeometryRay(*this); }
};

class GeometryCircle : public GeometryShape {
public:
    GeometryPoint pos;
    float radius;

    GeometryCircle(const GeometryPoint& pos = GeometryPoint(), float radius = 0.0f) : pos(pos), radius(radius) {}

    GeometryType getType() const override { return GeometryType::CIRCLE; }
    GeometryShape* clone() const override { return new GeometryCircle(*this); }
};

class GeometryRectangle : public GeometryShape {
public:
    GeometryPoint pos;
    GeometryPoint size;

    GeometryRectangle(const GeometryPoint& pos = GeometryPoint(), const GeometryPoint& size = GeometryPoint()) : pos(pos), size(size) {}

    GeometryType getType() const override { return GeometryType::RECTANGLE; }
    GeometryShape* clone() const override { return new GeometryRectangle(*this); }
};

class GeometryTriangle : public GeometryShape {
public:
    GeometryPoint a;
    GeometryPoint b;
    GeometryPoint c;

    GeometryTriangle(const GeometryPoint& a = GeometryPoint(), const GeometryPoint& b = GeometryPoint(), const GeometryPoint& c = GeometryPoint()) : a(a), b(b), c(c) {}

    GeometryType getType() const override { return GeometryType::TRIANGLE; }
    GeometryShape* clone() const override { return new GeometryTriangle(*this); }
};

std::vector<GeometryPoint> geometryGetIntersections(GeometryShape& first, GeometryShape& second);
bool geometryShapeContains(GeometryShape& first, GeometryShape& second);
GeometryRectangle getBoundingRectangle(const std::vector<GeometryShape>& shapes);
GeometryRectangle getBoundingRectangle(const GeometryShape* shape);
float distanceBetweenPoints(const GeometryPoint point1, const GeometryPoint point2);
std::vector<GeometryLineSegment> getLineSegmentsOfShape(const GeometryShape& shape);
glm::vec2 pointToVec(const GeometryPoint& p);

#endif // GEOMETRY_H
