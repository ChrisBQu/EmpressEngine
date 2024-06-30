#include "GameObj_TestWall.h"

#include "../Logger.h"

// Constructor
GameObject_Wall::GameObject_Wall() {
	visible = true;
	solid = true;
	is_static = true;

	x = 100;
	y = 0;
	transform->depth = 99;
	transform->rotation = 0;

	transform->scale = { 0.75, 0.75 };

	sprite->setStaticSprite("tex1", { 0, 32 }, { 32, 32 });
	collider->setRect({ { 0, 0 }, { 32, 32 } });

}

GameObject_Wall::~GameObject_Wall() { }
