#include "GameObj_TestWall.h"

#include "../Logger.h"

// Constructor
GameObject_Wall::GameObject_Wall() {
	visible = true;
	solid = true;
	is_static = false;

	transform->pos = { 100, 0 };
	transform->size = { 16, 16 };
	transform->depth = 99;
	transform->rotation = 0;

	sprite->setStaticSprite("tex1", { 0, 32 }, { 32, 32 });
	collider->setRect({ { -8, -8 }, { 16, 16 } });

}

GameObject_Wall::~GameObject_Wall() { }
