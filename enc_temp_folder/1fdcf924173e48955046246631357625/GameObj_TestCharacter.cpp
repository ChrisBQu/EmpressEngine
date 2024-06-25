#include "GameObj_TestCharacter.h"

#include "../Logger.h"
#include "../Scene.h"

// Constructor
GameObject_Character::GameObject_Character() {
	visible = true;
	solid = true;

	transform->pos = { 0, -100 };
	transform->size = { 32, 32 };
	transform->depth = 99;
	transform->rotation = 0;
	sprite->setAnimation("PINK_BLOB_WALK");
	collider->setRect({{ -16, -16 }, { 32, 32 }});
}

GameObject_Character::~GameObject_Character() { }


void GameObject_Character::onUpdate() {
	queryCollisions();
	if (myCollisions.size() > 0) {
		LOG("HONK");
	}

	float terminal_yspeed = 3.5;
	float y_acceleration = 0.75;
	GeometryPoint anchor = { transform->pos.x, transform->pos.y + transform->size.y / 2.0f - 1 };

	if (getLoadedScene()->getPlaceFree(anchor)) { yspeed += y_acceleration; }
	if (yspeed > terminal_yspeed) { yspeed = terminal_yspeed; }
	float d = getLoadedScene()->fireRay({ anchor, {0, 1} });
	if (d > yspeed) { transform->pos.y += yspeed; }
	else { transform->pos.y += d; }

}

void GameObject_Character::trigger_onPressedA() {
	yspeed -= 12;
}

void GameObject_Character::trigger_onPressedLeft() {
	sprite->setAnimation("PINK_BLOB_WALK");
	transform->size = { -32, 32 };

}
void GameObject_Character::trigger_onPressedRight() {
	sprite->setAnimation("PINK_BLOB_WALK");
	transform->size = { 32, 32 };

}

void GameObject_Character::trigger_onHeldLeft() {
	transform->pos = { transform->pos[0] - 2, transform->pos[1] };
}
void GameObject_Character::trigger_onHeldRight() { 
	transform->pos = { transform->pos[0] + 2, transform->pos[1] };
}

void GameObject_Character::trigger_onReleasedLeft() { 
	sprite->setAnimation("PINK_BLOB_STAND");

}
void GameObject_Character::trigger_onReleasedRight() {
	sprite->setAnimation("PINK_BLOB_STAND");
}
