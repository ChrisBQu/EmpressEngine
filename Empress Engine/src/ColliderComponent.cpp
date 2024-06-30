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

bool ColliderComponent::collidesWith(ColliderComponent* other)
{
	return (geometryGetIntersections(this->aabb, other->aabb).size() > 0);
}

void ColliderComponent::setRect(GeometryRectangle rect) {
	orig_aabb = rect;
}

void ColliderComponent::calibrate() {
	aabb.pos = { parent->x + orig_aabb.pos.x * parent->scale[0], parent->y + orig_aabb.pos.y * parent->scale[1]};
	aabb.size = { orig_aabb.size.x * parent->scale[0], orig_aabb.size.y * parent->scale[1] };
}
