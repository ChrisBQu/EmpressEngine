#include "Geometry.h"
#include "GameConfig.h"
#include "Logger.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <map>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Helper function to calculate the distance from a point to a line segment
float pointLineDistance(GeometryPoint p, GeometryPoint a, GeometryPoint b) {
    float A = p.x - a.x;
    float B = p.y - a.y;
    float C = b.x - a.x;
    float D = b.y - a.y;
    float dot = A * C + B * D;
    float len_sq = C * C + D * D;
    float param = (len_sq != 0) ? dot / len_sq : -1;
    float xx, yy;
    if (param < 0) {
        xx = a.x;
        yy = a.y;
    }
    else if (param > 1) {
        xx = b.x;
        yy = b.y;
    }
    else {
        xx = a.x + param * C;
        yy = a.y + param * D;
    }
    float dx = p.x - xx;
    float dy = p.y - yy;
    return std::sqrt(dx * dx + dy * dy);
}

// Helper function to check if two floats are approximately equal
bool almostEqual(float a, float b, float epsilon = 1e-5) {
    return std::abs(a - b) < epsilon;
}

// Helper contains function: Circle - Point
bool circleContainsPoint(const GeometryCircle& circle, const GeometryPoint& point) {
    float dx = point.x - circle.pos.x;
    float dy = point.y - circle.pos.y;
    return ((dx * dx + dy * dy) <= (circle.radius * circle.radius));
}

// Helper contains function: Circle - Circle
bool circleContainsCircle(const GeometryCircle& circle1, const GeometryCircle& circle2) {
    float dx = circle2.pos.x - circle1.pos.x;
    float dy = circle2.pos.y - circle1.pos.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return ((distance + circle2.radius) <= circle1.radius);
}

// Helper contains function: Circle - Line
bool circleContainsLine(const GeometryCircle& circle, const GeometryLineSegment& line) {
    return (circleContainsPoint(circle, line.start) && circleContainsPoint(circle, line.end));
}

// Helper contains function: Circle - Rectangle
bool circleContainsRectangle(const GeometryCircle& circle, const GeometryRectangle& rect) {
    GeometryPoint topLeft = { rect.pos.x, rect.pos.y };
    GeometryPoint topRight = { rect.pos.x + rect.size.x, rect.pos.y };
    GeometryPoint bottomLeft = { rect.pos.x, rect.pos.y + rect.size.y };
    GeometryPoint bottomRight = { rect.pos.x + rect.size.x, rect.pos.y + rect.size.y };
    return (circleContainsPoint(circle, topLeft) && circleContainsPoint(circle, topRight) && circleContainsPoint(circle, bottomLeft) && circleContainsPoint(circle, bottomRight));
}

// Helper contains function: Circle - Triangle
bool circleContainsTriangle(const GeometryCircle& circle, const GeometryTriangle& tri) {
    return (circleContainsPoint(circle, tri.a) && circleContainsPoint(circle, tri.b) && circleContainsPoint(circle, tri.c));
}

// Helper contains function: Rectangle - Point
bool rectangleContainsPoint(GeometryRectangle rect, GeometryPoint point) {
    return (point.x >= rect.pos.x && point.x <= rect.pos.x + rect.size.x && point.y >= rect.pos.y && point.y <= rect.pos.y + rect.size.y);
}

// Helper contains function: Rectangle - Circle
bool rectangleContainsCircle(const GeometryRectangle& rect, const GeometryCircle& circle) {
    GeometryPoint left = { circle.pos.x - circle.radius, circle.pos.y };
    GeometryPoint right = { circle.pos.x + circle.radius, circle.pos.y };
    GeometryPoint top = { circle.pos.x, circle.pos.y - circle.radius };
    GeometryPoint bottom = { circle.pos.x, circle.pos.y + circle.radius };
    return (rectangleContainsPoint(rect, left) && rectangleContainsPoint(rect, right) &&
        rectangleContainsPoint(rect, top) && rectangleContainsPoint(rect, bottom));
}

// Helper contains function: Rectangle - Line
bool rectangleContainsLine(const GeometryRectangle& rect, const GeometryLineSegment& line) {
    return (rectangleContainsPoint(rect, line.start) && rectangleContainsPoint(rect, line.end));
}

// Helper contains function: Rectangle - Rectangle
bool rectangleContainsRectangle(const GeometryRectangle& rect1, const GeometryRectangle& rect2) {
    GeometryPoint topLeft = { rect2.pos.x, rect2.pos.y };
    GeometryPoint topRight = { rect2.pos.x + rect2.size.x, rect2.pos.y };
    GeometryPoint bottomLeft = { rect2.pos.x, rect2.pos.y + rect2.size.y };
    GeometryPoint bottomRight = { rect2.pos.x + rect2.size.x, rect2.pos.y + rect2.size.y };
    return (rectangleContainsPoint(rect1, topLeft) && rectangleContainsPoint(rect1, topRight) && rectangleContainsPoint(rect1, bottomLeft) && rectangleContainsPoint(rect1, bottomRight));
}

// Helper contains function: Rectangle - Triangle
bool rectangleContainsTriangle(const GeometryRectangle& rect, const GeometryTriangle& tri) {
    return (rectangleContainsPoint(rect, tri.a) && rectangleContainsPoint(rect, tri.b) && rectangleContainsPoint(rect, tri.c));
}

// Helper contains function: Triangle - Point
bool triangleContainsPoint(const GeometryTriangle& tri, const GeometryPoint& point) {
    float d1, d2, d3;
    bool has_neg, has_pos;
    auto sign = [](const GeometryPoint& p1, const GeometryPoint& p2, const GeometryPoint& p3) {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
        };
    d1 = sign(point, tri.a, tri.b);
    d2 = sign(point, tri.b, tri.c);
    d3 = sign(point, tri.c, tri.a);
    has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    return !(has_neg && has_pos);
}

// Helper contains function: Triangle - Line
bool triangleContainsLine(const GeometryTriangle& tri, const GeometryLineSegment& line) {
    return (triangleContainsPoint(tri, line.start) && triangleContainsPoint(tri, line.end));
}

// Helper contains function: Triangle - Circle
bool triangleContainsCircle(const GeometryTriangle& tri, const GeometryCircle& circle) {
    if (!triangleContainsPoint(tri, circle.pos)) { return false; }
    if (pointLineDistance(circle.pos, tri.a, tri.b) < circle.radius) return false;
    if (pointLineDistance(circle.pos, tri.b, tri.c) < circle.radius) return false;
    if (pointLineDistance(circle.pos, tri.c, tri.a) < circle.radius) return false;
    return true;
}

// Helper contains function: Triangle - Rectangle
bool triangleContainsRectangle(const GeometryTriangle& tri, const GeometryRectangle& rect) {
    GeometryPoint topLeft = { rect.pos.x, rect.pos.y };
    GeometryPoint topRight = { rect.pos.x + rect.size.x, rect.pos.y };
    GeometryPoint bottomLeft = { rect.pos.x, rect.pos.y + rect.size.y };
    GeometryPoint bottomRight = { rect.pos.x + rect.size.x, rect.pos.y + rect.size.y };
    return (triangleContainsPoint(tri, topLeft) && triangleContainsPoint(tri, topRight) &&
        triangleContainsPoint(tri, bottomLeft) && triangleContainsPoint(tri, bottomRight));
}

// Helper contains function: Triangle - Triangle
bool triangleContainsTriangle(const GeometryTriangle& tri1, const GeometryTriangle& tri2) {
    return triangleContainsPoint(tri1, tri2.a) &&
        triangleContainsPoint(tri1, tri2.b) &&
        triangleContainsPoint(tri1, tri2.c);
}

// Helper intersection function: Point - Point
std::vector<GeometryPoint> intersectsPoints(GeometryPoint point1, GeometryPoint point2) {
    std::vector<GeometryPoint> intersections;
    if (point1.x == point2.x && point1.y == point2.y) { intersections.push_back(point1); }
    return intersections;
}

// Helper intersection function: Line - Line
std::vector<GeometryPoint> intersectsLines(GeometryLineSegment line1, GeometryLineSegment line2) {
    std::vector<GeometryPoint> intersections;
    float x1 = line1.start.x, y1 = line1.start.y;
    float x2 = line1.end.x, y2 = line1.end.y;
    float x3 = line2.start.x, y3 = line2.start.y;
    float x4 = line2.end.x, y4 = line2.end.y;
    float denom = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
    if (almostEqual(denom, 0.0f)) { return intersections; }
    float ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denom;
    float ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denom;
    if (ua >= 0.0f && ua <= 1.0f && ub >= 0.0f && ub <= 1.0f) {
        GeometryPoint intersection;
        intersection.x = x1 + ua * (x2 - x1);
        intersection.y = y1 + ua * (y2 - y1);
        intersections.push_back(intersection);
    }
    return intersections;
}

// Helper intersection function: Circle - Circle
std::vector<GeometryPoint> intersectsCircles(GeometryCircle circle1, GeometryCircle circle2) {
    std::vector<GeometryPoint> intersections;

    float dx = circle2.pos.x - circle1.pos.x;
    float dy = circle2.pos.y - circle1.pos.y;
    float d = std::sqrt(dx * dx + dy * dy);

    if (d > circle1.radius + circle2.radius || d < std::abs(circle1.radius - circle2.radius)) {
        return intersections; 
    }

    float a = (circle1.radius * circle1.radius - circle2.radius * circle2.radius + d * d) / (2 * d);
    float h = std::sqrt(circle1.radius * circle1.radius - a * a);
    float xm = circle1.pos.x + a * dx / d;
    float ym = circle1.pos.y + a * dy / d;
    float xs1 = xm + h * dy / d;
    float ys1 = ym - h * dx / d;
    float xs2 = xm - h * dy / d;
    float ys2 = ym + h * dx / d;

    intersections.push_back({ xs1, ys1 });
    intersections.push_back({ xs2, ys2 });

    return intersections;

}

// Helper intersection function: Rectangle - Rectangle
std::vector<GeometryPoint> intersectsRectangles(GeometryRectangle rect1, GeometryRectangle rect2) {
    std::vector<GeometryPoint> intersections;
    float x_overlap = std::max(0.0f, std::min(rect1.pos.x + rect1.size.x, rect2.pos.x + rect2.size.x) - std::max(rect1.pos.x, rect2.pos.x));
    float y_overlap = std::max(0.0f, std::min(rect1.pos.y + rect1.size.y, rect2.pos.y + rect2.size.y) - std::max(rect1.pos.y, rect2.pos.y));
    if (x_overlap > 0 && y_overlap > 0) {
        intersections.push_back({ std::max(rect1.pos.x, rect2.pos.x), std::max(rect1.pos.y, rect2.pos.y) });
        intersections.push_back({ std::min(rect1.pos.x + rect1.size.x, rect2.pos.x + rect2.size.x), std::min(rect1.pos.y + rect1.size.y, rect2.pos.y + rect2.size.y) });
    }
    return intersections;
}

// Helper intersection function: Triangle - Triangle
std::vector<GeometryPoint> intersectsTriangles(GeometryTriangle tri1, GeometryTriangle tri2) {
    std::vector<GeometryPoint> intersections;
    if (triangleContainsPoint(tri2, tri1.a)) intersections.push_back(tri1.a);
    if (triangleContainsPoint(tri2, tri1.b)) intersections.push_back(tri1.b);
    if (triangleContainsPoint(tri2, tri1.c)) intersections.push_back(tri1.c);
    if (triangleContainsPoint(tri1, tri2.a)) intersections.push_back(tri2.a);
    if (triangleContainsPoint(tri1, tri2.b)) intersections.push_back(tri2.b);
    if (triangleContainsPoint(tri1, tri2.c)) intersections.push_back(tri2.c);
    GeometryLineSegment edges1[] = { {tri1.a, tri1.b}, {tri1.b, tri1.c}, {tri1.c, tri1.a} };
    GeometryLineSegment edges2[] = { {tri2.a, tri2.b}, {tri2.b, tri2.c}, {tri2.c, tri2.a} };
    for (auto& edge1 : edges1) {
        for (auto& edge2 : edges2) {
            auto points = intersectsLines(edge1, edge2);
            intersections.insert(intersections.end(), points.begin(), points.end());
        }
    }
    return intersections;
}

// Helper intersection function: Ray - Ray
std::vector<GeometryPoint> intersectsRays(GeometryRay ray1, GeometryRay ray2) {
    std::vector<GeometryPoint> intersections;
    float x1 = ray1.start.x, y1 = ray1.start.y;
    float x2 = x1 + ray1.direction.x, y2 = y1 + ray1.direction.y;
    float x3 = ray2.start.x, y3 = ray2.start.y;
    float x4 = x3 + ray2.direction.x, y4 = y3 + ray2.direction.y;
    float denom = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
    if (almostEqual(denom, 0.0f)) { return intersections; }
    float ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denom;
    float ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denom;
    if (ua >= 0.0f && ub >= 0.0f) {
        GeometryPoint intersection;
        intersection.x = x1 + ua * (x2 - x1);
        intersection.y = y1 + ua * (y2 - y1);
        intersections.push_back(intersection);
    }
    return intersections;
}

// Helper intersection function: Point - Line
std::vector<GeometryPoint> intersectsPointLine(GeometryPoint point, GeometryLineSegment line) {
    std::vector<GeometryPoint> intersections;
    float x1 = line.start.x;
    float y1 = line.start.y;
    float x2 = line.end.x;
    float y2 = line.end.y;
    float dx = x2 - x1;
    float dy = y2 - y1;
    float length = std::sqrt(dx * dx + dy * dy);
    float projection = ((point.x - x1) * dx + (point.y - y1) * dy) / (length * length);
    if (projection >= 0.0f && projection <= 1.0f) {
        float closestX = x1 + projection * dx;
        float closestY = y1 + projection * dy;
        if (almostEqual(closestX, point.x) && almostEqual(closestY, point.y)) {
            intersections.push_back(point);
        }
    }
    return intersections;
}

// Helper intersection function: Point - Circle
std::vector<GeometryPoint> intersectsPointCircle(GeometryPoint point, GeometryCircle circle) {
    std::vector<GeometryPoint> intersections;
    float dx = point.x - circle.pos.x;
    float dy = point.y - circle.pos.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    if (almostEqual(distance, circle.radius)) {
        intersections.push_back(point);
    }
    return intersections;
}

// Helper intersection function: Point - Rectangle
std::vector<GeometryPoint> intersectsPointRectangle(GeometryPoint point, GeometryRectangle rect) {
    std::vector<GeometryPoint> intersections;
    if (point.x >= rect.pos.x && point.x <= rect.pos.x + rect.size.x &&
        point.y >= rect.pos.y && point.y <= rect.pos.y + rect.size.y) {
        intersections.push_back(point);
    }
    return intersections;
}

// Helper intersection function: Line - Rectangle
std::vector<GeometryPoint> intersectsLineRectangle(GeometryLineSegment line, GeometryRectangle rect) {
    std::vector<GeometryPoint> intersections;
    GeometryLineSegment top = { rect.pos, {rect.pos.x + rect.size.x, rect.pos.y} };
    GeometryLineSegment bottom = { {rect.pos.x, rect.pos.y + rect.size.y}, {rect.pos.x + rect.size.x, rect.pos.y + rect.size.y} };
    GeometryLineSegment left = { rect.pos, {rect.pos.x, rect.pos.y + rect.size.y} };
    GeometryLineSegment right = { {rect.pos.x + rect.size.x, rect.pos.y}, {rect.pos.x + rect.size.x, rect.pos.y + rect.size.y} };
    std::vector<GeometryPoint> topIntersections = intersectsLines(line, top);
    std::vector<GeometryPoint> bottomIntersections = intersectsLines(line, bottom);
    std::vector<GeometryPoint> leftIntersections = intersectsLines(line, left);
    std::vector<GeometryPoint> rightIntersections = intersectsLines(line, right);
    intersections.insert(intersections.end(), topIntersections.begin(), topIntersections.end());
    intersections.insert(intersections.end(), bottomIntersections.begin(), bottomIntersections.end());
    intersections.insert(intersections.end(), leftIntersections.begin(), leftIntersections.end());
    intersections.insert(intersections.end(), rightIntersections.begin(), rightIntersections.end());
    return intersections;
}

// Helper intersection function: Line - Circle
std::vector<GeometryPoint> intersectsLineCircle(GeometryLineSegment line, GeometryCircle circle) {
    std::vector<GeometryPoint> intersections;

    float x1 = line.start.x - circle.pos.x;
    float y1 = line.start.y - circle.pos.y;
    float x2 = line.end.x - circle.pos.x;
    float y2 = line.end.y - circle.pos.y;

    float dx = x2 - x1;
    float dy = y2 - y1;
    float dr = std::sqrt(dx * dx + dy * dy);
    float D = x1 * y2 - x2 * y1;
    float discriminant = circle.radius * circle.radius * dr * dr - D * D;

    if (discriminant < 0.0f) {
        return intersections; // No intersection
    }

    float sqrtDiscriminant = std::sqrt(discriminant);
    float signDy = (dy < 0.0f) ? -1.0f : 1.0f;

    GeometryPoint intersection1, intersection2;
    intersection1.x = (D * dy + signDy * dx * sqrtDiscriminant) / (dr * dr) + circle.pos.x;
    intersection1.y = (-D * dx + std::abs(dy) * sqrtDiscriminant) / (dr * dr) + circle.pos.y;
    intersection2.x = (D * dy - signDy * dx * sqrtDiscriminant) / (dr * dr) + circle.pos.x;
    intersection2.y = (-D * dx - std::abs(dy) * sqrtDiscriminant) / (dr * dr) + circle.pos.y;

    if (almostEqual(discriminant, 0.0f)) {
        intersections.push_back(intersection1);
    }
    else {
        intersections.push_back(intersection1);
        intersections.push_back(intersection2);
    }

    return intersections;
}

// Helper intersection function: Circle - Rectangle
std::vector<GeometryPoint> intersectsCircleRectangle(GeometryCircle circle, GeometryRectangle rect) {
    std::vector<GeometryPoint> intersections;

    GeometryLineSegment top = { rect.pos, {rect.pos.x + rect.size.x, rect.pos.y} };
    GeometryLineSegment bottom = { {rect.pos.x, rect.pos.y + rect.size.y}, {rect.pos.x + rect.size.x, rect.pos.y + rect.size.y} };
    GeometryLineSegment left = { rect.pos, {rect.pos.x, rect.pos.y + rect.size.y} };
    GeometryLineSegment right = { {rect.pos.x + rect.size.x, rect.pos.y}, {rect.pos.x + rect.size.x, rect.pos.y + rect.size.y} };

    auto filterIntersections = [](const GeometryLineSegment& line, const std::vector<GeometryPoint>& points) {
        std::vector<GeometryPoint> validPoints;
        for (const auto& point : points) {
            if (point.x >= std::min(line.start.x, line.end.x) && point.x <= std::max(line.start.x, line.end.x) &&
                point.y >= std::min(line.start.y, line.end.y) && point.y <= std::max(line.start.y, line.end.y)) {
                validPoints.push_back(point);
            }
        }
        return validPoints;
        };

    std::vector<GeometryPoint> topIntersections = filterIntersections(top, intersectsLineCircle(top, circle));
    std::vector<GeometryPoint> bottomIntersections = filterIntersections(bottom, intersectsLineCircle(bottom, circle));
    std::vector<GeometryPoint> leftIntersections = filterIntersections(left, intersectsLineCircle(left, circle));
    std::vector<GeometryPoint> rightIntersections = filterIntersections(right, intersectsLineCircle(right, circle));

    intersections.insert(intersections.end(), topIntersections.begin(), topIntersections.end());
    intersections.insert(intersections.end(), bottomIntersections.begin(), bottomIntersections.end());
    intersections.insert(intersections.end(), leftIntersections.begin(), leftIntersections.end());
    intersections.insert(intersections.end(), rightIntersections.begin(), rightIntersections.end());

    return intersections;
}

// Helper intersection function: Ray - Point
std::vector<GeometryPoint> intersectsRayPoint(GeometryRay ray, GeometryPoint point) {
    std::vector<GeometryPoint> intersections;
    float t = (point.x - ray.start.x) / ray.direction.x;
    if (t >= 0.0f && almostEqual(ray.start.y + t * ray.direction.y, point.y)) {
        intersections.push_back(point);
    }
    return intersections;
}

// Helper intersection function: Ray - Line
std::vector<GeometryPoint> intersectsRayLine(GeometryRay ray, GeometryLineSegment line) {
    std::vector<GeometryPoint> intersections;
    float x1 = ray.start.x, y1 = ray.start.y;
    float x2 = x1 + ray.direction.x, y2 = y1 + ray.direction.y;
    float x3 = line.start.x, y3 = line.start.y;
    float x4 = line.end.x, y4 = line.end.y;
    float denom = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
    if (almostEqual(denom, 0.0f)) { return intersections; }
    float ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denom;
    float ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denom;
    if (ua >= 0.0f && ub >= 0.0f && ub <= 1.0f) {
        GeometryPoint intersection;
        intersection.x = x1 + ua * (x2 - x1);
        intersection.y = y1 + ua * (y2 - y1);
        intersections.push_back(intersection);
    }
    return intersections;
}

// Helper intersection function: Ray - Circle
std::vector<GeometryPoint> intersectsRayCircle(GeometryRay ray, GeometryCircle circle) {
    std::vector<GeometryPoint> intersections;
    float x1 = ray.start.x - circle.pos.x;
    float y1 = ray.start.y - circle.pos.y;
    float dx = ray.direction.x;
    float dy = ray.direction.y;
    float dr = std::sqrt(dx * dx + dy * dy);
    float D = x1 * dy - y1 * dx;
    float discriminant = circle.radius * circle.radius * dr * dr - D * D;
    if (discriminant < 0.0f) { return intersections; }
    float sqrtDiscriminant = std::sqrt(discriminant);
    float signDy = (dy < 0.0f) ? -1.0f : 1.0f;
    GeometryPoint intersection1, intersection2;
    intersection1.x = (D * dy + signDy * dx * sqrtDiscriminant) / (dr * dr) + circle.pos.x;
    intersection1.y = (-D * dx + std::abs(dy) * sqrtDiscriminant) / (dr * dr) + circle.pos.y;
    intersection2.x = (D * dy - signDy * dx * sqrtDiscriminant) / (dr * dr) + circle.pos.x;
    intersection2.y = (-D * dx - std::abs(dy) * sqrtDiscriminant) / (dr * dr) + circle.pos.y;
    float t1 = (intersection1.x - ray.start.x) / dx;
    float t2 = (intersection2.x - ray.start.x) / dx;
    if (t1 >= 0.0f) {
        intersections.push_back(intersection1);
    }
    if (t2 >= 0.0f) {
        intersections.push_back(intersection2);
    }
    return intersections;
}

// Helper intersection function: Ray - Rectangle
std::vector<GeometryPoint> intersectsRayRectangle(GeometryRay ray, GeometryRectangle rect) {
    std::vector<GeometryPoint> intersections;
    GeometryLineSegment top = { rect.pos, {rect.pos.x + rect.size.x, rect.pos.y} };
    GeometryLineSegment bottom = { {rect.pos.x, rect.pos.y + rect.size.y}, {rect.pos.x + rect.size.x, rect.pos.y + rect.size.y} };
    GeometryLineSegment left = { rect.pos, {rect.pos.x, rect.pos.y + rect.size.y} };
    GeometryLineSegment right = { {rect.pos.x + rect.size.x, rect.pos.y}, {rect.pos.x + rect.size.x, rect.pos.y + rect.size.y} };
    std::vector<GeometryPoint> topIntersections = intersectsRayLine(ray, top);
    std::vector<GeometryPoint> bottomIntersections = intersectsRayLine(ray, bottom);
    std::vector<GeometryPoint> leftIntersections = intersectsRayLine(ray, left);
    std::vector<GeometryPoint> rightIntersections = intersectsRayLine(ray, right);
    intersections.insert(intersections.end(), topIntersections.begin(), topIntersections.end());
    intersections.insert(intersections.end(), bottomIntersections.begin(), bottomIntersections.end());
    intersections.insert(intersections.end(), leftIntersections.begin(), leftIntersections.end());
    intersections.insert(intersections.end(), rightIntersections.begin(), rightIntersections.end());
    return intersections;
}

// Helper intersection function: Triangle - Point
std::vector<GeometryPoint> intersectsTrianglePoint(GeometryTriangle tri, GeometryPoint point) {
    std::vector<GeometryPoint> intersections;
    if (triangleContainsPoint(tri, point)) {
        intersections.push_back(point);
    }
    return intersections;
}

// Helper intersection function: Triangle - Line
std::vector<GeometryPoint> intersectsTriangleLine(GeometryTriangle tri, GeometryLineSegment line) {
    std::vector<GeometryPoint> intersections;
    GeometryLineSegment edges[] = { {tri.a, tri.b}, {tri.b, tri.c}, {tri.c, tri.a} };
    for (auto& edge : edges) {
        auto points = intersectsLines(edge, line);
        intersections.insert(intersections.end(), points.begin(), points.end());
    }
    return intersections;
}

// Helper intersection function: Triangle - Circle
std::vector<GeometryPoint> intersectsTriangleCircle(GeometryTriangle tri, GeometryCircle circle) {
    std::vector<GeometryPoint> intersections;
    GeometryLineSegment edges[] = { {tri.a, tri.b}, {tri.b, tri.c}, {tri.c, tri.a} };
    for (auto& edge : edges) {
        auto points = intersectsLineCircle(edge, circle);
        intersections.insert(intersections.end(), points.begin(), points.end());
    }
    return intersections;
}

// Helper intersection function: Triangle - Rectangle
std::vector<GeometryPoint> intersectsTriangleRectangle(GeometryTriangle tri, GeometryRectangle rect) {
    std::vector<GeometryPoint> intersections;
    GeometryPoint topRight = { rect.pos.x + rect.size.x, rect.pos.y };
    GeometryPoint bottomLeft = { rect.pos.x, rect.pos.y + rect.size.y };
    GeometryPoint bottomRight = { rect.pos.x + rect.size.x, rect.pos.y + rect.size.y };

    GeometryLineSegment rectEdges[] = {
        {rect.pos, topRight},
        {bottomLeft, bottomRight},
        {rect.pos, bottomLeft},
        {topRight, bottomRight}
    };
    GeometryLineSegment triEdges[] = { {tri.a, tri.b}, {tri.b, tri.c}, {tri.c, tri.a} };
    for (auto& triEdge : triEdges) {
        for (auto& rectEdge : rectEdges) {
            auto points = intersectsLines(triEdge, rectEdge);
            intersections.insert(intersections.end(), points.begin(), points.end());
        }
    }
    return intersections;
}

// Helper intersection function: Triangle - Ray
std::vector<GeometryPoint> intersectsTriangleRay(GeometryTriangle tri, GeometryRay ray) {
    std::vector<GeometryPoint> intersections;
    GeometryLineSegment edges[] = { {tri.a, tri.b}, {tri.b, tri.c}, {tri.c, tri.a} };
    for (auto& edge : edges) {
        auto points = intersectsRayLine(ray, edge);
        intersections.insert(intersections.end(), points.begin(), points.end());
    }
    return intersections;
}

// Return a vector of points of intersection between two shapes
std::vector<GeometryPoint> geometryGetIntersections(GeometryShape& first, GeometryShape& second) {
    std::vector<GeometryPoint> intersections;
    if (first.getType() == GeometryType::TRIANGLE) {
        const auto& triangle = dynamic_cast<const GeometryTriangle&>(first);
        if (second.getType() == GeometryType::TRIANGLE) { intersections = intersectsTriangles(triangle, dynamic_cast<const GeometryTriangle&>(second)); }
        else if (second.getType() == GeometryType::POINT) { intersections = intersectsTrianglePoint(triangle, dynamic_cast<const GeometryPoint&>(second)); }
        else if (second.getType() == GeometryType::LINE_SEGMENT) { intersections = intersectsTriangleLine(triangle, dynamic_cast<const GeometryLineSegment&>(second)); }
        else if (second.getType() == GeometryType::RECTANGLE) { intersections = intersectsTriangleRectangle(triangle, dynamic_cast<const GeometryRectangle&>(second)); }
        else if (second.getType() == GeometryType::CIRCLE) { intersections = intersectsTriangleCircle(triangle, dynamic_cast<const GeometryCircle&>(second)); }
        else if (second.getType() == GeometryType::RAY) { intersections = intersectsTriangleRay(triangle, dynamic_cast<const GeometryRay&>(second)); }
    }
    else if (first.getType() == GeometryType::RAY) {
        const auto& ray = dynamic_cast<const GeometryRay&>(first);
        if (second.getType() == GeometryType::RAY) { intersections = intersectsRays(ray, dynamic_cast<const GeometryRay&>(second)); }
        else if (second.getType() == GeometryType::TRIANGLE) { intersections = intersectsTriangleRay(dynamic_cast<const GeometryTriangle&>(second), ray); }
        else if (second.getType() == GeometryType::POINT) { intersections = intersectsRayPoint(ray, dynamic_cast<const GeometryPoint&>(second)); }
        else if (second.getType() == GeometryType::LINE_SEGMENT) { intersections = intersectsRayLine(ray, dynamic_cast<const GeometryLineSegment&>(second)); }
        else if (second.getType() == GeometryType::RECTANGLE) { intersections = intersectsRayRectangle(ray, dynamic_cast<const GeometryRectangle&>(second)); }
        else if (second.getType() == GeometryType::CIRCLE) { intersections = intersectsRayCircle(ray, dynamic_cast<const GeometryCircle&>(second)); }
    }
    else if (first.getType() == GeometryType::POINT) {
        const auto& point = dynamic_cast<const GeometryPoint&>(first);
        if (second.getType() == GeometryType::POINT) { intersections = intersectsPoints(point, dynamic_cast<const GeometryPoint&>(second)); }
        else if (second.getType() == GeometryType::TRIANGLE) { intersections = intersectsTrianglePoint(dynamic_cast<const GeometryTriangle&>(second), point); }
        else if (second.getType() == GeometryType::LINE_SEGMENT) { intersections = intersectsPointLine(point, dynamic_cast<const GeometryLineSegment&>(second)); }
        else if (second.getType() == GeometryType::RECTANGLE) { intersections = intersectsPointRectangle(point, dynamic_cast<const GeometryRectangle&>(second)); }
        else if (second.getType() == GeometryType::CIRCLE) { intersections = intersectsPointCircle(point, dynamic_cast<const GeometryCircle&>(second)); }
        else if (second.getType() == GeometryType::RAY) { intersections = intersectsRayPoint(dynamic_cast<const GeometryRay&>(second), point); }

    }
    else if (first.getType() == GeometryType::LINE_SEGMENT) {
        const auto& line = dynamic_cast<const GeometryLineSegment&>(first);
        if (second.getType() == GeometryType::LINE_SEGMENT) { intersections = intersectsLines(line, dynamic_cast<const GeometryLineSegment&>(second)); }
        else if (second.getType() == GeometryType::TRIANGLE) { intersections = intersectsTriangleLine(dynamic_cast<const GeometryTriangle&>(second), line); }
        else if (second.getType() == GeometryType::RECTANGLE) { intersections = intersectsLineRectangle(line, dynamic_cast<const GeometryRectangle&>(second)); }
        else if (second.getType() == GeometryType::CIRCLE) { intersections = intersectsLineCircle(line, dynamic_cast<const GeometryCircle&>(second)); }
        else if (second.getType() == GeometryType::POINT) { intersections = intersectsPointLine(dynamic_cast<const GeometryPoint&>(second), line); }
        else if (second.getType() == GeometryType::RAY) { intersections = intersectsRayLine(dynamic_cast<const GeometryRay&>(second), line); }

    }
    else if (first.getType() == GeometryType::CIRCLE) {
        const auto& circle = dynamic_cast<const GeometryCircle&>(first);
        if (second.getType() == GeometryType::RECTANGLE) { intersections = intersectsCircleRectangle(circle, dynamic_cast<const GeometryRectangle&>(second)); }
        else if (second.getType() == GeometryType::TRIANGLE) { intersections = intersectsTriangleCircle(dynamic_cast<const GeometryTriangle&>(second), circle); }
        else if (second.getType() == GeometryType::CIRCLE) { intersections = intersectsCircles(circle, dynamic_cast<const GeometryCircle&>(second)); }
        else if (second.getType() == GeometryType::POINT) { intersections = intersectsPointCircle(dynamic_cast<const GeometryPoint&>(second), circle); }
        else if (second.getType() == GeometryType::LINE_SEGMENT) { intersections = intersectsLineCircle(dynamic_cast<const GeometryLineSegment&>(second), circle); }
        else if (second.getType() == GeometryType::RAY) { intersections = intersectsRayCircle(dynamic_cast<const GeometryRay&>(second), circle); }
    }
    else if (first.getType() == GeometryType::RECTANGLE) {
        const auto& rect = dynamic_cast<const GeometryRectangle&>(first);
        if (second.getType() == GeometryType::RECTANGLE) { intersections = intersectsRectangles(rect, dynamic_cast<const GeometryRectangle&>(second)); }
        else if (second.getType() == GeometryType::TRIANGLE) { intersections = intersectsTriangleRectangle(dynamic_cast<const GeometryTriangle&>(second), rect); }
        else if (second.getType() == GeometryType::POINT) { intersections = intersectsPointRectangle(dynamic_cast<const GeometryPoint&>(second), rect); }
        else if (second.getType() == GeometryType::LINE_SEGMENT) { intersections = intersectsLineRectangle(dynamic_cast<const GeometryLineSegment&>(second), rect); }
        else if (second.getType() == GeometryType::CIRCLE) { intersections = intersectsCircleRectangle(dynamic_cast<const GeometryCircle&>(second), rect); }
        else if (second.getType() == GeometryType::RAY) { intersections = intersectsRayRectangle(dynamic_cast<const GeometryRay&>(second), rect); }
    }
    return intersections;
}


// Return true or false for whether one shape contains another
bool geometryShapeContains(GeometryShape &first, GeometryShape &second) {
    if (first.getType() == GeometryType::RECTANGLE) {
        const auto& rect = dynamic_cast<const GeometryRectangle&>(first);
        if (second.getType() == GeometryType::POINT) { return rectangleContainsPoint(rect, dynamic_cast<const GeometryPoint&>(second)); }
        if (second.getType() == GeometryType::LINE_SEGMENT) { return rectangleContainsLine(rect, dynamic_cast<const GeometryLineSegment&>(second)); }
        if (second.getType() == GeometryType::RECTANGLE) { return rectangleContainsRectangle(rect, dynamic_cast<const GeometryRectangle&>(second)); }
        if (second.getType() == GeometryType::CIRCLE) { return rectangleContainsCircle(rect, dynamic_cast<const GeometryCircle&>(second)); }
        if (second.getType() == GeometryType::TRIANGLE) { return rectangleContainsTriangle(rect, dynamic_cast<const GeometryTriangle&>(second)); }
    }
    if (first.getType() == GeometryType::CIRCLE) {
        const auto& circle = dynamic_cast<const GeometryCircle&>(first);
        if (second.getType() == GeometryType::POINT) { return circleContainsPoint(circle, dynamic_cast<const GeometryPoint&>(second)); }
        if (second.getType() == GeometryType::LINE_SEGMENT) { return circleContainsLine(circle, dynamic_cast<const GeometryLineSegment&>(second)); }
        if (second.getType() == GeometryType::RECTANGLE) { return circleContainsRectangle(circle, dynamic_cast<const GeometryRectangle&>(second)); }
        if (second.getType() == GeometryType::CIRCLE) { return circleContainsCircle(circle, dynamic_cast<const GeometryCircle&>(second)); }
        if (second.getType() == GeometryType::TRIANGLE) { return circleContainsTriangle(circle, dynamic_cast<const GeometryTriangle&>(second)); }
    }
    if (first.getType() == GeometryType::TRIANGLE) {
        const auto& triangle = dynamic_cast<const GeometryTriangle&>(first);
        if (second.getType() == GeometryType::POINT) { return triangleContainsPoint(triangle, dynamic_cast<const GeometryPoint&>(second)); }
        if (second.getType() == GeometryType::LINE_SEGMENT) { return triangleContainsLine(triangle, dynamic_cast<const GeometryLineSegment&>(second)); }
        if (second.getType() == GeometryType::RECTANGLE) { return triangleContainsRectangle(triangle, dynamic_cast<const GeometryRectangle&>(second)); }
        if (second.getType() == GeometryType::CIRCLE) { return triangleContainsCircle(triangle, dynamic_cast<const GeometryCircle&>(second)); }
        if (second.getType() == GeometryType::TRIANGLE) { return triangleContainsTriangle(triangle, dynamic_cast<const GeometryTriangle&>(second)); }
    }
    return false;
}

// Helper function to calculate the bounding box for a single shape
GeometryRectangle calculateBoundingBoxForShape(const GeometryShape* shape) {
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();

    switch (shape->getType()) {
    case GeometryType::POINT: {
        const auto& point = dynamic_cast<const GeometryPoint&>(*shape);
        minX = std::min(minX, point.x);
        minY = std::min(minY, point.y);
        maxX = std::max(maxX, point.x);
        maxY = std::max(maxY, point.y);
        break;
    }
    case GeometryType::LINE_SEGMENT: {
        const auto& line = dynamic_cast<const GeometryLineSegment&>(*shape);
        minX = std::min({ minX, line.start.x, line.end.x });
        minY = std::min({ minY, line.start.y, line.end.y });
        maxX = std::max({ maxX, line.start.x, line.end.x });
        maxY = std::max({ maxY, line.start.y, line.end.y });
        break;
    }
    case GeometryType::CIRCLE: {
        const auto& circle = dynamic_cast<const GeometryCircle&>(*shape);
        minX = std::min(minX, circle.pos.x - circle.radius);
        minY = std::min(minY, circle.pos.y - circle.radius);
        maxX = std::max(maxX, circle.pos.x + circle.radius);
        maxY = std::max(maxY, circle.pos.y + circle.radius);
        break;
    }
    case GeometryType::RECTANGLE: {
        const auto& rect = dynamic_cast<const GeometryRectangle&>(*shape);
        minX = std::min(minX, rect.pos.x);
        minY = std::min(minY, rect.pos.y);
        maxX = std::max(maxX, rect.pos.x + rect.size.x);
        maxY = std::max(maxY, rect.pos.y + rect.size.y);
        break;
    }
    case GeometryType::TRIANGLE: {
        const auto& tri = dynamic_cast<const GeometryTriangle&>(*shape);
        minX = std::min({ minX, tri.a.x, tri.b.x, tri.c.x });
        minY = std::min({ minY, tri.a.y, tri.b.y, tri.c.y });
        maxX = std::max({ maxX, tri.a.x, tri.b.x, tri.c.x });
        maxY = std::max({ maxY, tri.a.y, tri.b.y, tri.c.y });
        break;
    }
    default:
        break;
    }

    return { {minX, minY}, {maxX - minX, maxY - minY} };
}

// Function to generate the bounding box for a vector of shapes
GeometryRectangle getBoundingRectangle(const std::vector<GeometryShape*>& shapes) {
    if (shapes.empty()) { return { {0, 0}, {0, 0} }; }

    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();

    for (const auto& shape : shapes) {
        GeometryRectangle boundingBox = calculateBoundingBoxForShape(shape);
        minX = std::min(minX, boundingBox.pos.x);
        minY = std::min(minY, boundingBox.pos.y);
        maxX = std::max(maxX, boundingBox.pos.x + boundingBox.size.x);
        maxY = std::max(maxY, boundingBox.pos.y + boundingBox.size.y);
    }

    return { {minX, minY}, {maxX - minX, maxY - minY} };
}

// Function to generate the bounding box for a single shape
GeometryRectangle getBoundingRectangle(const GeometryShape* shape) {
    if (!shape) { return { {0, 0}, {0, 0} }; }

    return calculateBoundingBoxForShape(shape);
}

// Function to get the distance between two GeometryPoint objects
float distanceBetweenPoints(const GeometryPoint point1, const GeometryPoint point2) {
    float dx = point2.x - point1.x;
    float dy = point2.y - point1.y;
    return std::sqrt(dx * dx + dy * dy);
}

// Function to get line segments from a GeometryShape
std::vector<GeometryLineSegment> getLineSegmentsOfShape(const GeometryShape& shape) {
    std::vector<GeometryLineSegment> lineSegments;

    switch (shape.getType()) {
    case GeometryType::LINE_SEGMENT: {
        const auto& line = dynamic_cast<const GeometryLineSegment&>(shape);
        lineSegments.push_back(line);
        break;
    }
    case GeometryType::RECTANGLE: {
        const auto& rect = dynamic_cast<const GeometryRectangle&>(shape);
        GeometryPoint topLeft{ rect.pos.x, rect.pos.y };
        GeometryPoint topRight{ rect.pos.x + rect.size.x, rect.pos.y };
        GeometryPoint bottomLeft{ rect.pos.x, rect.pos.y + rect.size.y };
        GeometryPoint bottomRight{ rect.pos.x + rect.size.x, rect.pos.y + rect.size.y };
        lineSegments.push_back({ topLeft, topRight });
        lineSegments.push_back({ topRight, bottomRight });
        lineSegments.push_back({ bottomRight, bottomLeft });
        lineSegments.push_back({ bottomLeft, topLeft });
        break;
    }
    case GeometryType::TRIANGLE: {
        const auto& tri = dynamic_cast<const GeometryTriangle&>(shape);
        lineSegments.push_back({ tri.a, tri.b });
        lineSegments.push_back({ tri.b, tri.c });
        lineSegments.push_back({ tri.c, tri.a });
        break;
    }
    case GeometryType::CIRCLE: {
        const auto& circle = dynamic_cast<const GeometryCircle&>(shape);
        const int numSegments = LINE_SEGMENTS_TO_APPROXIMATE_CIRCLE;
        const float angleIncrement = (float)(2.0f * M_PI / numSegments);
        GeometryPoint prevPoint{
            (float)(circle.pos.x + circle.radius * std::cos(0)),
            (float)(circle.pos.y + circle.radius * std::sin(0))
        };
        for (int i = 1; i <= numSegments; ++i) {
            float angle = i * angleIncrement;
            GeometryPoint newPoint{
                (float)(circle.pos.x + circle.radius * std::cos(angle)),
                (float)(circle.pos.y + circle.radius * std::sin(angle))
            };
            lineSegments.push_back({ prevPoint, newPoint });
            prevPoint = newPoint;
        }
        break;
    }
    default:
        break;
    }

    return lineSegments;
}

// Convert a GeomePoint to a glm::vec2
glm::vec2 pointToVec(const GeometryPoint& p) { return { p.x, p.y }; }
