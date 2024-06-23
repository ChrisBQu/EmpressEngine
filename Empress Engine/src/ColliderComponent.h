#ifndef COLLIDER_COMPONENT_H
#define COLLIDER_COMPONENT_H

#include "Geometry.h"

#include <vector>

// Forward declaration
class GameObject;

class ColliderComponent {
public:
	ColliderComponent(GameObject* p);
	GeometryRectangle getAABB();
	bool collidesWith(ColliderComponent* other);
	void setRect(GeometryRectangle rect);
	void calibrate();
private:
	GeometryRectangle aabb;
	GeometryRectangle orig_aabb;
	GameObject* parent;
};

#endif // COLLIDER_COMPONENT_H