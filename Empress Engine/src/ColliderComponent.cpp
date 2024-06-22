#include "ColliderComponent.h"
#include "GameObject.h"

ColliderComponent::ColliderComponent(GameObject* p) {
	parent = p;
}

void ColliderComponent::update() {
	aabb = { {parent->transform->pos[0], parent->transform->pos[1]}, {parent->transform->size[0], parent->transform->size[1] } };
}
