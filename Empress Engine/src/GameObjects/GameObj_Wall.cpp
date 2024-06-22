#include "GameObj_TestWall.h"

#include "../Logger.h"

// Constructor
GameObject_Wall::GameObject_Wall() {
	visible = true;
	solid = true;

	transform->pos = { 100, 0 };
	transform->size = { 32, 32 };
	transform->depth = 99;
	transform->rotation = 0;
	sprite->setStaticSprite("tex1", { 0, 32 }, { 32, 32 });

	GeometryShape mask;
	mask.shapetype = GeometryType::RECTANGLE;
	mask.shape.rectangle.pos = { -16, -16 };
	mask.shape.rectangle.size = { 32, 32 };
	std::vector<GeometryShape> gs = { mask };

	collider->setCollisionShapes(gs);
}

GameObject_Wall::~GameObject_Wall() { }
