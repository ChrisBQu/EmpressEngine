#include "GameObj_TestCharacter.h"

#include "../Logger.h"
#include "../Scene.h"

// Constructor
GameObject_Character::GameObject_Character() {
	visible = true;
	solid = true;

	x = 0;
	y = -32;
	transform->depth = 99;
	transform->rotation = 0;
	sprite->setAnimation("PINK_BLOB_WALK");
	collider->setRect({{0, 0}, { 32, 32 }});

	transform->scale = { 0.5, 0.5 };

}

GameObject_Character::~GameObject_Character() { }


void GameObject_Character::onUpdate() {
	queryCollisions();
	if (myCollisions.size() > 0) {
		LOG("HONK");
	}


	float terminal_yspeed = 3.5;
	float y_acceleration = 0.25;
	GeometryPoint anchor1 = { collider->getAABB().pos.x, collider->getAABB().pos.y + collider->getAABB().size.y };
	GeometryPoint anchor2 = { collider->getAABB().pos.x + collider->getAABB().size.x, collider->getAABB().pos.y + collider->getAABB().size.y };


	if (getLoadedScene()->getPlaceFree(anchor1) && getLoadedScene()->getPlaceFree(anchor2)) { yspeed += y_acceleration; }
	if (yspeed > terminal_yspeed) { yspeed = terminal_yspeed; }
	float d = getLoadedScene()->fireRay({ anchor1, {0, 1} });
	float d2 = getLoadedScene()->fireRay({ anchor2, {0, 1} });
	d = std::min(d, d2);
	if (d > yspeed) { y += yspeed; }
	else { y += d; }
	
}

void GameObject_Character::trigger_onPressedA() {
	yspeed -= 8;
}

void GameObject_Character::trigger_onPressedLeft() {
	sprite->setAnimation("PINK_BLOB_WALK");
	transform->scale = { -0.5, 0.5 };

}
void GameObject_Character::trigger_onPressedRight() {
	sprite->setAnimation("PINK_BLOB_WALK");
	transform->scale = { 0.5, 0.5 };

}

void GameObject_Character::trigger_onHeldLeft() {
	x -= 2;
}
void GameObject_Character::trigger_onHeldRight() { 
	x += 2;
}

void GameObject_Character::trigger_onReleasedLeft() { 
	sprite->setAnimation("PINK_BLOB_STAND");

}
void GameObject_Character::trigger_onReleasedRight() {
	sprite->setAnimation("PINK_BLOB_STAND");
}
