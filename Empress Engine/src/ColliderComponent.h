#ifndef COLLIDER_COMPONENT_H
#define COLLIDER_COMPONENT_H

#include "Geometry.h"

#include <vector>

// Forward declaration
class GameObject;

class ColliderComponent {
public:
	ColliderComponent(GameObject* p);
	~ColliderComponent();
	GeometryRectangle getAABB();
	GeometryShape* getShape();
	bool collidesWithAABB(ColliderComponent* other);
	bool collidesWith(ColliderComponent* other);
	void setShape(GeometryShape &s);
	void calibrate();
private:
	GeometryRectangle aabb;
	GeometryRectangle orig_aabb;
	GeometryShape *shape;
	GeometryShape *orig_shape;
	GameObject* parent;
};

#endif // COLLIDER_COMPONENT_H