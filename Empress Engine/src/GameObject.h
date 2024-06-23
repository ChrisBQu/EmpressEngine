#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "ColliderComponent.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"

#include <vector>

class GameObject {
public:
	GameObject();
	virtual ~GameObject();

	void queryCollisions();

	void update();
	void render();
	virtual void onUpdate();
	virtual void onRender();

	virtual void trigger_onPressedUp();
	virtual void trigger_onPressedDown();
	virtual void trigger_onPressedLeft();
	virtual void trigger_onPressedRight();
	virtual void trigger_onPressedA();
	virtual void trigger_onPressedB();
	virtual void trigger_onPressedX();
	virtual void trigger_onPressedY();
	virtual void trigger_onPressedL();
	virtual void trigger_onPressedR();
	virtual void trigger_onPressedStart();
	virtual void trigger_onPressedBack();

	virtual void trigger_onHeldUp();
	virtual void trigger_onHeldDown();
	virtual void trigger_onHeldLeft();
	virtual void trigger_onHeldRight();
	virtual void trigger_onHeldA();
	virtual void trigger_onHeldB();
	virtual void trigger_onHeldX();
	virtual void trigger_onHeldY();
	virtual void trigger_onHeldL();
	virtual void trigger_onHeldR();
	virtual void trigger_onHeldStart();
	virtual void trigger_onHeldBack();

	virtual void trigger_onReleasedUp();
	virtual void trigger_onReleasedDown();
	virtual void trigger_onReleasedLeft();
	virtual void trigger_onReleasedRight();
	virtual void trigger_onReleasedA();
	virtual void trigger_onReleasedB();
	virtual void trigger_onReleasedX();
	virtual void trigger_onReleasedY();
	virtual void trigger_onReleasedL();
	virtual void trigger_onReleasedR();
	virtual void trigger_onReleasedStart();
	virtual void trigger_onReleasedBack();

	// Variables
	uint32_t id;
	glm::vec2 scale;

	bool visible;
	bool solid;

	bool is_static;

	float alpha;

	std::vector<GameObject*> myCollisions;

	// Components
	TransformComponent *transform;
	SpriteComponent *sprite;
	ColliderComponent *collider;
};

#endif // GAME_OBJECT_H