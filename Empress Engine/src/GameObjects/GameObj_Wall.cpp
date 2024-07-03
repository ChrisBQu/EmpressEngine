#include "GameObj_TestWall.h"

#include "../Logger.h"

// Constructor
GameObject_Wall::GameObject_Wall() {
	visible = true;
	solid = true;
	transform->depth = 99;
	transform->rotation = 0;

	GeometryRectangle rect = { {0,0}, {32, 32} };
	collider->setShape(rect);

	transform->scale = { 0.5, 0.5 };

	sprite->setStaticSprite("tex1", { 0, 32 }, { 32, 32 });

}

GameObject_Wall::~GameObject_Wall() { }
