#include "ColliderComponent.h"
#include "GameObject.h"
#include "Geometry.h"

#include "Logger.h"

ColliderComponent::ColliderComponent(GameObject* p) {
	parent = p;
}

GeometryRectangle ColliderComponent::getAABB() {
	return aabb;
}

bool ColliderComponent::collidesWith(ColliderComponent* other){
	return (geometryGetIntersections(aabb, other->aabb).size() > 0);
}

void ColliderComponent::setRect(GeometryRectangle rect) {
	orig_aabb = rect;
}
void ColliderComponent::calibrate() {
    float sx = parent->transform->scale[0];
    float sy = parent->transform->scale[1];
    float posX = parent->x - parent->sprite->getWidth()/2.0f*sx;
    float posY = parent->y - parent->sprite->getHeight()/2.0*sy;
    float width = orig_aabb.size.x * sx;
    float height = orig_aabb.size.y * sy;
    aabb.pos = { posX, posY };
    aabb.size = { width, height };
}





