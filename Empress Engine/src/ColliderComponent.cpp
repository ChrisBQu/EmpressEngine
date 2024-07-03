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

// Call once per frame to adjust the collider's shape to match the parent's scale and position
void ColliderComponent::calibrate() {
    float sx = parent->transform->scale[0];
    float sy = parent->transform->scale[1];
    float posX = parent->x - parent->sprite->getWidth()/2.0f * sx;
    float posY = parent->y - parent->sprite->getHeight()/2.0 * sy;
    float width = orig_aabb.size.x * sx;
    float height = orig_aabb.size.y * sy;

    aabb.pos = { posX, posY };
    aabb.size = { width, height };

    if (shape != nullptr) {
        if (shape->getType() == GeometryType::CIRCLE) {
            GeometryCircle* ptr = (GeometryCircle*)shape;
            GeometryCircle* orig = (GeometryCircle*)orig_shape;
            ptr->pos.x = posX + orig->pos.x * sx;
            ptr->pos.y = posY + orig->pos.y * sy;
            ptr->radius = abs(orig->radius * sx);
        }
        
        else if (shape->getType() == GeometryType::TRIANGLE) {
            GeometryTriangle* ptr = (GeometryTriangle*)shape;
            GeometryTriangle* orig = (GeometryTriangle*)orig_shape;
            ptr->a.x = posX + orig->a.x * sx;
            ptr->a.y = posY + orig->a.y * sy;
            ptr->b.x = posX + orig->b.x * sx;
            ptr->b.y = posY + orig->b.y * sy;
            ptr->c.x = posX + orig->c.x * sx;
            ptr->c.y = posY + orig->c.y * sy;
        }

        else if (shape->getType() == GeometryType::RECTANGLE) {
            GeometryRectangle* ptr = (GeometryRectangle*)shape;
            GeometryRectangle* orig = (GeometryRectangle*)orig_shape;
            ptr->pos.x = posX + orig->pos.x * sx;
            ptr->pos.y = posY + orig->pos.y * sy;
            ptr->size.x = orig->size.x * sx;
            ptr->size.y = orig->size.y * sy;
        }

        else if (shape->getType() == GeometryType::POINT) {
            GeometryPoint* ptr = (GeometryPoint*)shape;
            GeometryPoint* orig = (GeometryPoint*)orig_shape;
            ptr->x = posX + orig->x * sy;
            ptr->y = posY + orig->y * sy;
        }

        else if (shape->getType() == GeometryType::LINE_SEGMENT) {
            GeometryLineSegment* ptr = (GeometryLineSegment*)shape;
            GeometryLineSegment* orig = (GeometryLineSegment*)orig_shape;
            ptr->start.x = posX + orig->start.x * sy;
            ptr->start.y = posY + orig->start.y * sy;
            ptr->end.x = posX + orig->end.x * sy;
            ptr->end.y = posY + orig->end.y * sy;
        }

        aabb = getBoundingRectangle(shape);

    }

}





