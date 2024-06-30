#include "TransformComponent.h"

// Constructor
TransformComponent::TransformComponent(GameObject* p) {
	scale = { 1, 1 };
	depth = 0;
	rotation = 0;

	// Currently the parent doesn't do anything with this structure, but we are setting it so that all components have a parent
	parent = p;

}
