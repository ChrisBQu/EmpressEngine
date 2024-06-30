#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <glm/glm.hpp>

// Forward declaration
class GameObject;

class TransformComponent {
public:
	TransformComponent(GameObject* p);
	glm::vec2 scale;
	int depth;
	float rotation;
private:
	GameObject *parent;
};

#endif // TRANSFORM_COMPONENT_H