#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include <glm/glm.hpp>

// Forward declaration
class GameObject;

class TransformComponent {
public:
	TransformComponent(GameObject* p);
	glm::ivec2 pos;
	glm::ivec2 scale;
	int depth;
	float rotation;
private:
	GameObject *parent;
};

#endif // TRANSFORM_COMPONENT_H