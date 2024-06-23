#include "ColliderComponent.h"
#include "GameObject.h"
#include "Logger.h"

ColliderComponent::ColliderComponent(GameObject* p) {
	parent = p;
}

GeometryRectangle ColliderComponent::getAABB() {
	return aabb;
}

bool ColliderComponent::collidesWith(ColliderComponent* other)
{
	for (GeometryShape my_shape : collisionShapes) {
		for (GeometryShape other_shape : other->collisionShapes) {
			if (geometryGetIntersections(my_shape, other_shape).size() > 0) { return true; }
		}
	}
	return false;
}

void ColliderComponent::setCollisionShapes(std::vector<GeometryShape> shapes) {
	collisionShapes.clear();
	collisionShapes = shapes;
	orig_aabb = getBoundingRectangle(collisionShapes);
}

void ColliderComponent::calibrate() {
	aabb.pos = { parent->transform->pos[0] + orig_aabb.pos.x * parent->scale[0], parent->transform->pos[1] + orig_aabb.pos.y * parent->scale[1]};
	aabb.size = { orig_aabb.size.x * parent->scale[0], orig_aabb.size.y * parent->scale[1] };
}
