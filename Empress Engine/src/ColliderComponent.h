#ifndef COLLIDER_COMPONENT_H
#define COLLIDER_COMPONENT_H

#include "Geometry.h"

#include <vector>

// Forward declaration
class GameObject;

class ColliderComponent {
public:
	ColliderComponent(GameObject* p);
	void update();
private:
	GeometryRectangle aabb;
	std::vector<GeometryShape> collisionShapes;
	GameObject* parent;
};

#endif // COLLIDER_COMPONENT_H