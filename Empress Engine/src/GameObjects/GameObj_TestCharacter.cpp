#include "GameObj_TestCharacter.h"

#include "../Logger.h"
#include "../Scene.h"

// Constructor
GameObject_Character::GameObject_Character() {
	visible = true;
	solid = true;

	x = -32;
	y = 32;
	transform->depth = 99;
	transform->rotation = 0;
	sprite->setAnimation("PINK_BLOB_WALK");

	//GeometryCircle circ = { {16, 16}, 16 };
	//collider->setShape(circ);

	//GeometryTriangle tri = { {0,0}, {32, 32}, {0, 32} };
	//collider->setShape(tri);

	GeometryRectangle rect = { {0,0}, {32, 32} };
	collider->setShape(rect);

	transform->scale = { 0.5, 0.5 };

}

GameObject_Character::~GameObject_Character() { }


void GameObject_Character::onUpdate() {

	queryCollisions();
	if (myCollisions.size() > 0) {
		LOG("HONK");
	}

	/*
	float terminal_yspeed = 3;
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

	*/
	
}

void GameObject_Character::trigger_onPressedA() {
	//GeometryPoint anchor1 = { collider->getAABB().pos.x, collider->getAABB().pos.y + collider->getAABB().size.y };
	//GeometryPoint anchor2 = { collider->getAABB().pos.x + collider->getAABB().size.x, collider->getAABB().pos.y + collider->getAABB().size.y };
	//if (!getLoadedScene()->getPlaceFree(anchor1) || !getLoadedScene()->getPlaceFree(anchor2)) { yspeed -= 8; }
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
	moveX(-3);
}
void GameObject_Character::trigger_onHeldRight() { 
	moveX(3);
}
void GameObject_Character::trigger_onHeldUp() {
	moveY(-3);
}
void GameObject_Character::trigger_onHeldDown() {
	moveY(3);
}


void GameObject_Character::trigger_onReleasedLeft() { 
	sprite->setAnimation("PINK_BLOB_STAND");

}
void GameObject_Character::trigger_onReleasedRight() {
	sprite->setAnimation("PINK_BLOB_STAND");
}
