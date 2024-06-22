#include "GameObj_TestCharacter.h"

#include "../Logger.h"

// Constructor
GameObject_Character::GameObject_Character() {
	visible = true;
	transform->pos = { 0, 0 };
	transform->size = { 32, 32 };
	transform->depth = 99;
	transform->rotation = 0;
	sprite->setAnimation("PINK_BLOB_WALK");
}

GameObject_Character::~GameObject_Character() { }


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
