#include "ColliderComponent.h"
#include "GameObject.h"
#include "Geometry.h"
#include "Logger.h"

// Constructor
ColliderComponent::ColliderComponent(GameObject* p) {
	parent = p;
}

// Destructor
ColliderComponent::~ColliderComponent() {
    if (shape != nullptr) { delete shape; }
    if (orig_shape != nullptr) { delete orig_shape; }
}

// Get the collider's bounding box
GeometryRectangle ColliderComponent::getAABB() {
	return aabb;
}

// Get the collider's shape
GeometryShape* ColliderComponent::getShape() {
    return shape;
}

// Check one collider against another for broad collision using AABB
bool ColliderComponent::collidesWithAABB(ColliderComponent* other){
    if (shape == nullptr || other->shape == nullptr) { return false; }
	return (geometryGetIntersections(aabb, other->aabb).size() > 0);
}

// Check one collider against another for narrow collision using actual shape of colliders
bool ColliderComponent::collidesWith(ColliderComponent* other) {
    if (shape == nullptr || other->shape == nullptr) { return false; }
    return (geometryGetIntersections(*shape, *other->shape).size() > 0);
}

// Set the shape of the collider
void ColliderComponent::setShape(GeometryShape &s) {
    if (s.getType() == GeometryType::RAY) {
        LOG_ERROR("Could not set collider for object with ID (", parent->id, "). RAY is not a valid shape for colliders.");
        return;
    }
    if (shape != nullptr) { delete shape; }
    if (orig_shape != nullptr) { delete orig_shape; }
    shape = s.clone();
    orig_shape = s.clone();
}

void ColliderComponent::calibrate() {
    float sx = parent->transform->scale[0];
    float sy = parent->transform->scale[1];
    float halfWidth = parent->sprite->getWidth() / 2.0f;
    float halfHeight = parent->sprite->getHeight() / 2.0f;

    float posX = parent->x;
    float posY = parent->y;

    float width = orig_aabb.size.x * abs(sx);
    float height = orig_aabb.size.y * abs(sy);

    aabb.pos = { posX - halfWidth * sx, posY - halfHeight * sy };
    aabb.size = { width, height };

    if (shape != nullptr) {
        if (shape->getType() == GeometryType::CIRCLE) {
            GeometryCircle* ptr = (GeometryCircle*)shape;
            GeometryCircle* orig = (GeometryCircle*)orig_shape;
            ptr->pos.x = posX + (orig->pos.x - halfWidth) * sx;
            ptr->pos.y = posY + (orig->pos.y - halfHeight) * sy;
            ptr->radius = abs(orig->radius * sx);
        }

        else if (shape->getType() == GeometryType::TRIANGLE) {
            GeometryTriangle* ptr = (GeometryTriangle*)shape;
            GeometryTriangle* orig = (GeometryTriangle*)orig_shape;
            ptr->a.x = posX + (orig->a.x - halfWidth) * sx;
            ptr->a.y = posY + (orig->a.y - halfHeight) * sy;
            ptr->b.x = posX + (orig->b.x - halfWidth) * sx;
            ptr->b.y = posY + (orig->b.y - halfHeight) * sy;
            ptr->c.x = posX + (orig->c.x - halfWidth) * sx;
            ptr->c.y = posY + (orig->c.y - halfHeight) * sy;

            // Ensure vertices are correctly oriented when scale is negative
            if (sx < 0) {
                std::swap(ptr->a.x, ptr->b.x);
                std::swap(ptr->a.y, ptr->b.y);
            }
            if (sy < 0) {
                std::swap(ptr->a.x, ptr->c.x);
                std::swap(ptr->a.y, ptr->c.y);
            }
        }

        else if (shape->getType() == GeometryType::RECTANGLE) {
            float posX = parent->x - (halfWidth * (sx > 0 ? 1 : -1) * sx);
            float posY = parent->y - (halfHeight * (sy > 0 ? 1 : -1) * sy);
            float width = orig_aabb.size.x * abs(sx);
            float height = orig_aabb.size.y * abs(sy);
            aabb.pos = { posX, posY };
            aabb.size = { width, height };
            GeometryRectangle* ptr = (GeometryRectangle*)shape;
            GeometryRectangle* orig = (GeometryRectangle*)orig_shape;
            ptr->pos.x = posX + orig->pos.x * sx;
            ptr->pos.y = posY + orig->pos.y * sy;
            ptr->size.x = orig->size.x * abs(sx);
            ptr->size.y = orig->size.y * abs(sy);
        }


        else if (shape->getType() == GeometryType::POINT) {
            GeometryPoint* ptr = (GeometryPoint*)shape;
            GeometryPoint* orig = (GeometryPoint*)orig_shape;
            ptr->x = posX + (orig->x - halfWidth) * sx;
            ptr->y = posY + (orig->y - halfHeight) * sy;
        }

        else if (shape->getType() == GeometryType::LINE_SEGMENT) {
            GeometryLineSegment* ptr = (GeometryLineSegment*)shape;
            GeometryLineSegment* orig = (GeometryLineSegment*)orig_shape;
            ptr->start.x = posX + (orig->start.x - halfWidth) * sx;
            ptr->start.y = posY + (orig->start.y - halfHeight) * sy;
            ptr->end.x = posX + (orig->end.x - halfWidth) * sx;
            ptr->end.y = posY + (orig->end.y - halfHeight) * sy;

            // Ensure line segments are correctly oriented when scale is negative
            if (sx < 0) {
                std::swap(ptr->start.x, ptr->end.x);
                std::swap(ptr->start.y, ptr->end.y);
            }
            if (sy < 0) {
                std::swap(ptr->start.x, ptr->end.x);
                std::swap(ptr->start.y, ptr->end.y);
            }
        }

        aabb = getBoundingRectangle(shape);
    }
}
