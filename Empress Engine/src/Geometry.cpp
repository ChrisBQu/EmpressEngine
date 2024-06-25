#include "Geometry.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <map>

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
bool circleContainsPoint(GeometryCircle circle, GeometryPoint point) {
    float dx = point.x - circle.pos.x;
    float dy = point.y - circle.pos.y;
    return ((dx * dx + dy * dy) <= (circle.radius * circle.radius));
}

// Helper contains function: Circle - Circle
bool circleContainsCircle(GeometryCircle circle1, GeometryCircle circle2) {
    float dx = circle2.pos.x - circle1.pos.x;
    float dy = circle2.pos.y - circle1.pos.y;
    float distance = std::sqrt(dx * dx + dy * dy);
    return ((distance + circle2.radius) <= circle1.radius);
}

// Helper contains function: Circle - Line
bool circleContainsLine(GeometryCircle circle, GeometryLineSegment line) {
    return (circleContainsPoint(circle, line.start) && circleContainsPoint(circle, line.end));
}

// Helper contains function: Circle - Rectangle
bool circleContainsRectangle(GeometryCircle circle, GeometryRectangle rect) {
    GeometryPoint topLeft = { rect.pos.x, rect.pos.y };
    GeometryPoint topRight = { rect.pos.x + rect.size.x, rect.pos.y };
    GeometryPoint bottomLeft = { rect.pos.x, rect.pos.y + rect.size.y };
    GeometryPoint bottomRight = { rect.pos.x + rect.size.x, rect.pos.y + rect.size.y };
    return (circleContainsPoint(circle, topLeft) && circleContainsPoint(circle, topRight) && circleContainsPoint(circle, bottomLeft) && circleContainsPoint(circle, bottomRight));
}

// Helper coontains function: Circle - Triangle
bool circleContainsTriangle(GeometryCircle circle, GeometryTriangle tri) {
    return (circleContainsPoint(circle, tri.a) && circleContainsPoint(circle, tri.b) && circleContainsPoint(circle, tri.c));
}

// Helper contains function: Rectangle - Point
bool rectangleContainsPoint(GeometryRectangle rect, GeometryPoint point) {
    return (point.x >= rect.pos.x && point.x <= rect.pos.x + rect.size.x && point.y >= rect.pos.y && point.y <= rect.pos.y + rect.size.y);
}

// Helper contains function: Rectangle - Circle
bool rectangleContainsCircle(GeometryRectangle rect, GeometryCircle circle) {
    GeometryPoint left = { circle.pos.x - circle.radius, circle.pos.y };
    GeometryPoint right = { circle.pos.x + circle.radius, circle.pos.y };
    GeometryPoint top = { circle.pos.x, circle.pos.y - circle.radius };
    GeometryPoint bottom = { circle.pos.x, circle.pos.y + circle.radius };
    return (rectangleContainsPoint(rect, left) && rectangleContainsPoint(rect, right) && rectangleContainsPoint(rect, top) && rectangleContainsPoint(rect, bottom));
}

// Helper contains function: Rectangle - Line
bool rectangleContainsLine(GeometryRectangle rect, GeometryLineSegment line) {
    return (rectangleContainsPoint(rect, line.start) && rectangleContainsPoint(rect, line.end));
}

// Helper contains function: Rectangle - Rectangle
bool rectangleContainsRectangle(GeometryRectangle rect1, GeometryRectangle rect2) {
    GeometryPoint topLeft = { rect2.pos.x, rect2.pos.y };
    GeometryPoint topRight = { rect2.pos.x + rect2.size.x, rect2.pos.y };
    GeometryPoint bottomLeft = { rect2.pos.x, rect2.pos.y + rect2.size.y };
    GeometryPoint bottomRight = { rect2.pos.x + rect2.size.x, rect2.pos.y + rect2.size.y };
    return (rectangleContainsPoint(rect1, topLeft) && rectangleContainsPoint(rect1, topRight) && rectangleContainsPoint(rect1, bottomLeft) && rectangleContainsPoint(rect1, bottomRight));
}

// Helper contains function: Rectangle - Triangle
bool rectangleContainsTriangle(GeometryRectangle rect, GeometryTriangle tri) {
    return (rectangleContainsPoint(rect, tri.a) && rectangleContainsPoint(rect, tri.b) && rectangleContainsPoint(rect, tri.c));
}

// Helper contains function: Triangle - Point
bool triangleContainsPoint(GeometryTriangle tri, GeometryPoint point) {
    float d1, d2, d3;
    bool has_neg, has_pos;
    auto sign = [](GeometryPoint p1, GeometryPoint p2, GeometryPoint p3) {
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
bool triangleContainsLine(GeometryTriangle tri, GeometryLineSegment line) {
    return (triangleContainsPoint(tri, line.start) && triangleContainsPoint(tri, line.end));
}

// Helper contains function: Triangle - Circle
bool triangleContainsCircle(GeometryTriangle tri, GeometryCircle circle) {
    if (!triangleContainsPoint(tri, circle.pos)) { return false; }
    if (pointLineDistance(circle.pos, tri.a, tri.b) < circle.radius) return false;
    if (pointLineDistance(circle.pos, tri.b, tri.c) < circle.radius) return false;
    if (pointLineDistance(circle.pos, tri.c, tri.a) < circle.radius) return false;
    return true;
}

// Helper contains function: Triangle - Rectangle
bool triangleContainsRectangle(GeometryTriangle tri, GeometryRectangle rect) {
    GeometryPoint topLeft = { rect.pos.x, rect.pos.y };
    GeometryPoint topRight = { rect.pos.x + rect.size.x, rect.pos.y };
    GeometryPoint bottomLeft = { rect.pos.x, rect.pos.y + rect.size.y };
    GeometryPoint bottomRight = { rect.pos.x + rect.size.x, rect.pos.y + rect.size.y };
    return (triangleContainsPoint(tri, topLeft) && triangleContainsPoint(tri, topRight) && triangleContainsPoint(tri, bottomLeft) && triangleContainsPoint(tri, bottomRight));
}

// Helper contains function: Triangle - Triangle
bool triangleContainsTriangle(GeometryTriangle tri1, GeometryTriangle tri2) {
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
    if (discriminant < 0.0f) { return intersections; }
    float sqrtDiscriminant = std::sqrt(discriminant);
    float signDy = (dy < 0.0f) ? -1.0f : 1.0f;
    GeometryPoint intersection1, intersection2;
    intersection1.x = (D * dy + signDy * dx * sqrtDiscriminant) / (dr * dr) + circle.pos.x;
    intersection1.y = (-D * dx + std::abs(dy) * sqrtDiscriminant) / (dr * dr) + circle.pos.y;
    intersection2.x = (D * dy - signDy * dx * sqrtDiscriminant) / (dr * dr) + circle.pos.x;
    intersection2.y = (-D * dx - std::abs(dy) * sqrtDiscriminant) / (dr * dr) + circle.pos.y;
    if (almostEqual(discriminant, 0.0f)) { intersections.push_back(intersection1); }
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
    std::vector<GeometryPoint> topIntersections = intersectsLineCircle(top, circle);
    std::vector<GeometryPoint> bottomIntersections = intersectsLineCircle(bottom, circle);
    std::vector<GeometryPoint> leftIntersections = intersectsLineCircle(left, circle);
    std::vector<GeometryPoint> rightIntersections = intersectsLineCircle(right, circle);
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
    GeometryLineSegment rectEdges[] = {
        {rect.pos, {rect.pos.x + rect.size.x, rect.pos.y}},
        {{rect.pos.x, rect.pos.y + rect.size.y}, {rect.pos.x + rect.size.x, rect.pos.y + rect.size.y}},
        {rect.pos, {rect.pos.x, rect.pos.y + rect.size.y}},
        {{rect.pos.x + rect.size.x, rect.pos.y}, {rect.pos.x + rect.size.x, rect.pos.y + rect.size.y}}
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
std::vector<GeometryPoint> geometryGetIntersections(GeometryShape first, GeometryShape second) {
    std::vector<GeometryPoint> intersections;
    if (first.shapetype == GeometryType::TRIANGLE) {
        if (second.shapetype == GeometryType::TRIANGLE) { intersections = intersectsTriangles(first.shape.triangle, second.shape.triangle); }
        else if (second.shapetype == GeometryType::POINT) { intersections = intersectsTrianglePoint(first.shape.triangle, second.shape.point); }
        else if (second.shapetype == GeometryType::LINE_SEGMENT) { intersections = intersectsTriangleLine(first.shape.triangle, second.shape.line); }
        else if (second.shapetype == GeometryType::RECTANGLE) { intersections = intersectsTriangleRectangle(first.shape.triangle, second.shape.rectangle); }
        else if (second.shapetype == GeometryType::CIRCLE) { intersections = intersectsTriangleCircle(first.shape.triangle, second.shape.circle); }
        else if (second.shapetype == GeometryType::RAY) { intersections = intersectsTriangleRay(first.shape.triangle, second.shape.ray); }
    }
    else if (first.shapetype == GeometryType::RAY) {
        if (second.shapetype == GeometryType::RAY) { intersections = intersectsRays(first.shape.ray, second.shape.ray); }
        else if (second.shapetype == GeometryType::POINT) { intersections = intersectsRayPoint(first.shape.ray, second.shape.point); }
        else if (second.shapetype == GeometryType::LINE_SEGMENT) { intersections = intersectsRayLine(first.shape.ray, second.shape.line); }
        else if (second.shapetype == GeometryType::RECTANGLE) { intersections = intersectsRayRectangle(first.shape.ray, second.shape.rectangle); }
        else if (second.shapetype == GeometryType::CIRCLE) { intersections = intersectsRayCircle(first.shape.ray, second.shape.circle); }
    }
    else if (first.shapetype == GeometryType::POINT) {
        if (second.shapetype == GeometryType::POINT) { intersections = intersectsPoints(first.shape.point, second.shape.point); }
        else if (second.shapetype == GeometryType::LINE_SEGMENT) { intersections = intersectsPointLine(first.shape.point, second.shape.line); }
        else if (second.shapetype == GeometryType::RECTANGLE) { intersections = intersectsPointRectangle(first.shape.point, second.shape.rectangle); }
        else if (second.shapetype == GeometryType::CIRCLE) { intersections = intersectsPointCircle(first.shape.point, second.shape.circle); }
    }
    else if (first.shapetype == GeometryType::LINE_SEGMENT) {
        if (second.shapetype == GeometryType::LINE_SEGMENT) { intersections = intersectsLines(first.shape.line, second.shape.line); }
        else if (second.shapetype == GeometryType::RECTANGLE) { intersections = intersectsLineRectangle(first.shape.line, second.shape.rectangle); }
        else if (second.shapetype == GeometryType::CIRCLE) { intersections = intersectsLineCircle(first.shape.line, second.shape.circle); }
    }
    else if (first.shapetype == GeometryType::CIRCLE) {
        if (second.shapetype == GeometryType::RECTANGLE) { intersections = intersectsCircleRectangle(first.shape.circle, second.shape.rectangle); }
        else if (second.shapetype == GeometryType::CIRCLE) { intersections = intersectsCircles(first.shape.circle, second.shape.circle); }
    }
    else if (first.shapetype == GeometryType::RECTANGLE) {
        if (second.shapetype == GeometryType::RECTANGLE) { intersections = intersectsRectangles(first.shape.rectangle, second.shape.rectangle); }
    }
    return intersections;
}

// Return true or false for whether one shape contains another
bool geometryShapeContains(GeometryShape first, GeometryShape second) {
    std::vector<GeometryPoint> intersections;
    if (first.shapetype == GeometryType::RECTANGLE) {
        if (second.shapetype == GeometryType::POINT) { return rectangleContainsPoint(first.shape.rectangle, second.shape.point); }
        if (second.shapetype == GeometryType::LINE_SEGMENT) { return rectangleContainsLine(first.shape.rectangle, second.shape.line); }
        if (second.shapetype == GeometryType::RECTANGLE) { return rectangleContainsRectangle(first.shape.rectangle, second.shape.rectangle); }
        if (second.shapetype == GeometryType::CIRCLE) { return rectangleContainsCircle(first.shape.rectangle, second.shape.circle); }
        if (second.shapetype == GeometryType::TRIANGLE) { return rectangleContainsTriangle(first.shape.rectangle, second.shape.triangle); }
    }
    if (first.shapetype == GeometryType::CIRCLE) {
        if (second.shapetype == GeometryType::POINT) { return circleContainsPoint(first.shape.circle, second.shape.point); }
        if (second.shapetype == GeometryType::LINE_SEGMENT) { return circleContainsLine(first.shape.circle, second.shape.line); }
        if (second.shapetype == GeometryType::RECTANGLE) { return circleContainsRectangle(first.shape.circle, second.shape.rectangle); }
        if (second.shapetype == GeometryType::CIRCLE) { return circleContainsCircle(first.shape.circle, second.shape.circle); }
        if (second.shapetype == GeometryType::TRIANGLE) { return circleContainsTriangle (first.shape.circle, second.shape.triangle); }
    }
    if (first.shapetype == GeometryType::TRIANGLE) {
        if (second.shapetype == GeometryType::POINT) { return triangleContainsPoint(first.shape.triangle, second.shape.point); }
        if (second.shapetype == GeometryType::LINE_SEGMENT) { return triangleContainsLine(first.shape.triangle, second.shape.line); }
        if (second.shapetype == GeometryType::RECTANGLE) { return triangleContainsRectangle(first.shape.triangle, second.shape.rectangle); }
        if (second.shapetype == GeometryType::CIRCLE) { return triangleContainsCircle(first.shape.triangle, second.shape.circle); }
        if (second.shapetype == GeometryType::TRIANGLE) { return triangleContainsTriangle(first.shape.triangle, second.shape.triangle); }
    }
    return false;
}

// Generate the bounding box for a vector of shapes
GeometryRectangle getBoundingRectangle(const std::vector<GeometryShape>& shapes) {

    if (shapes.size() == 0) { return { {0,0}, {0, 0} }; }

    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float maxY = std::numeric_limits<float>::min();

    for (const auto& shape : shapes) {
        switch (shape.shapetype) {
        case GeometryType::POINT: {
            minX = std::min(minX, shape.shape.point.x);
            minY = std::min(minY, shape.shape.point.y);
            maxX = std::max(maxX, shape.shape.point.x);
            maxY = std::max(maxY, shape.shape.point.y);
            break;
        }
        case GeometryType::LINE_SEGMENT: {
            minX = std::min({ minX, shape.shape.line.start.x, shape.shape.line.end.x });
            minY = std::min({ minY, shape.shape.line.start.y, shape.shape.line.end.y });
            maxX = std::max({ maxX, shape.shape.line.start.x, shape.shape.line.end.x });
            maxY = std::max({ maxY, shape.shape.line.start.y, shape.shape.line.end.y });
            break;
        }
        case GeometryType::CIRCLE: {
            minX = std::min(minX, shape.shape.circle.pos.x - shape.shape.circle.radius);
            minY = std::min(minY, shape.shape.circle.pos.y - shape.shape.circle.radius);
            maxX = std::max(maxX, shape.shape.circle.pos.x + shape.shape.circle.radius);
            maxY = std::max(maxY, shape.shape.circle.pos.y + shape.shape.circle.radius);
            break;
        }
        case GeometryType::RECTANGLE: {
            minX = std::min(minX, shape.shape.rectangle.pos.x);
            minY = std::min(minY, shape.shape.rectangle.pos.y);
            maxX = std::max(maxX, shape.shape.rectangle.pos.x + shape.shape.rectangle.size.x);
            maxY = std::max(maxY, shape.shape.rectangle.pos.y + shape.shape.rectangle.size.y);
            break;
        }
        case GeometryType::TRIANGLE: {
            minX = std::min({ minX, shape.shape.triangle.a.x, shape.shape.triangle.b.x, shape.shape.triangle.c.x });
            minY = std::min({ minY, shape.shape.triangle.a.y, shape.shape.triangle.b.y, shape.shape.triangle.c.y });
            maxX = std::max({ maxX, shape.shape.triangle.a.x, shape.shape.triangle.b.x, shape.shape.triangle.c.x });
            maxY = std::max({ maxY, shape.shape.triangle.a.y, shape.shape.triangle.b.y, shape.shape.triangle.c.y });
            break;
        }
        default:
            break;
        }
    }

    GeometryRectangle boundingRect;
    boundingRect.pos.x = minX;
    boundingRect.pos.y = minY;
    boundingRect.size.x = maxX - minX;
    boundingRect.size.y = maxY - minY;

    return boundingRect;
}

// Return a new shape that is a shape translated by a given amount
GeometryShape translateShape(GeometryShape& shape, float translateX, float translateY) {
    GeometryShape translatedShape = shape;

    switch (shape.shapetype) {
    case GeometryType::POINT: {
        translatedShape.shape.point.x += translateX;
        translatedShape.shape.point.y += translateY;
        break;
    }
    case GeometryType::LINE_SEGMENT: {
        translatedShape.shape.line.start.x += translateX;
        translatedShape.shape.line.start.y += translateY;
        translatedShape.shape.line.end.x += translateX;
        translatedShape.shape.line.end.y += translateY;
        break;
    }
    case GeometryType::CIRCLE: {
        translatedShape.shape.circle.pos.x += translateX;
        translatedShape.shape.circle.pos.y += translateY;
        break;
    }
    case GeometryType::RECTANGLE: {
        translatedShape.shape.rectangle.pos.x += translateX;
        translatedShape.shape.rectangle.pos.y += translateY;
        break;
    }
    case GeometryType::TRIANGLE: {
        translatedShape.shape.triangle.a.x += translateX;
        translatedShape.shape.triangle.a.y += translateY;
        translatedShape.shape.triangle.b.x += translateX;
        translatedShape.shape.triangle.b.y += translateY;
        translatedShape.shape.triangle.c.x += translateX;
        translatedShape.shape.triangle.c.y += translateY;
        break;
    }
    default:
        break;
    }

    return translatedShape;
}

float distanceBetweenPoints(const GeometryPoint point1, const GeometryPoint point2) {
    float dx = point2.x - point1.x;
    float dy = point2.y - point1.y;
    return std::sqrt(dx * dx + dy * dy);
}