#include "GameObj_TestCharacter.h"

#include "../Logger.h"

// Constructor
GameObject_Character::GameObject_Character() {
	visible = true;
	transform.pos = { 0, 0 };
	transform.scale = { 1.0, 1.0 };
	transform.depth = 0;
	transform.rotation = 0;
	transform.alpha = 1.0;

	sprite.setAnimation("PINK_BLOB_WALK");
}

GameObject_Character::~GameObject_Character() { }

void GameObject_Character::trigger_onPressedA() {
	LOG("JUMP!");
}