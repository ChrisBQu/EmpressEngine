#include "GameObject.h"
#include "GameConfig.h"
#include "Logger.h"
#include "RenderInterface.h"
#include "Scene.h"

#include <cmath>

uint64_t objectCount = 1;

GameObject::GameObject() {
	id = objectCount++;
	visible = false;
	solid = false;
	alpha = 1.0;
	sprite = new SpriteComponent(this);
	transform = new TransformComponent(this);
	collider = new ColliderComponent(this);
}

GameObject::~GameObject() { 
	delete sprite;
	delete transform;
	delete collider;
}

void GameObject::queryCollisions() {
	myCollisions = getLoadedScene()->queryCollisions(this);
}

void GameObject::update() {
	if (solid) { collider->calibrate(); }
	onUpdate();
}

void GameObject::render() {
	sprite->tickAnimation();
	if (visible) {
		sprite->render();
		onRender();
	}
	if (DRAW_BOUNDING_BOXES) {
		std::vector<GeometryLineSegment> lines = getLineSegmentsOfShape(collider->getAABB());
		for (GeometryLineSegment each_line : lines) {
			drawLine(pointToVec(each_line.start), pointToVec(each_line.end), 2, { 0.0, 1.0, 0.0, 1.0 });
		}

		GeometryShape* shp = collider->getShape();
		if (shp != nullptr) {
			std::vector<GeometryLineSegment> lines = getLineSegmentsOfShape(*shp);
			for (GeometryLineSegment each_line : lines) {
				drawLine(pointToVec(each_line.start), pointToVec(each_line.end), 2, { 0.0, 0.0, 1.0, 1.0 });
			}
		}


	}
}

void GameObject::onUpdate() { }
void GameObject::onRender() { }

void GameObject::trigger_onPressedUp() { }
void GameObject::trigger_onPressedDown() { }
void GameObject::trigger_onPressedLeft() { }
void GameObject::trigger_onPressedRight() { }
void GameObject::trigger_onPressedA() { }
void GameObject::trigger_onPressedB() { }
void GameObject::trigger_onPressedX() { }
void GameObject::trigger_onPressedY() { }
void GameObject::trigger_onPressedL() { }
void GameObject::trigger_onPressedR() { }
void GameObject::trigger_onPressedStart() { }
void GameObject::trigger_onPressedBack() { }

void GameObject::trigger_onHeldUp() { }
void GameObject::trigger_onHeldDown() { }
void GameObject::trigger_onHeldLeft() { }
void GameObject::trigger_onHeldRight() { }
void GameObject::trigger_onHeldA() { }
void GameObject::trigger_onHeldB() { }
void GameObject::trigger_onHeldX() { }
void GameObject::trigger_onHeldY() { }
void GameObject::trigger_onHeldL() { }
void GameObject::trigger_onHeldR() { }
void GameObject::trigger_onHeldStart() { }
void GameObject::trigger_onHeldBack() { }

void GameObject::trigger_onReleasedUp() { }
void GameObject::trigger_onReleasedDown() { }
void GameObject::trigger_onReleasedLeft() { }
void GameObject::trigger_onReleasedRight() { }
void GameObject::trigger_onReleasedA() { }
void GameObject::trigger_onReleasedB() { }
void GameObject::trigger_onReleasedX() { }
void GameObject::trigger_onReleasedY() { }
void GameObject::trigger_onReleasedL() { }
void GameObject::trigger_onReleasedR() { }
void GameObject::trigger_onReleasedStart() { }
void GameObject::trigger_onReleasedBack() { }

void GameObject::moveX(float amount) {
	GeometryShape* projShape = nullptr;
	float move = 0.0f;
	int sign = (amount > 0) ? 1 : -1;

	while (abs(move + sign) <= abs(amount)) {
		projShape = collider->getShape()->clone();
		projShape->translate(move + sign, 0);
		bool projRes = getLoadedScene()->projectShapeFree(projShape);
		if (!projRes) {
			delete projShape;
			x += move;
			return;
		}
		move += sign;
		delete projShape;
	}

	// Final check for the exact amount
	projShape = collider->getShape()->clone();
	projShape->translate(amount, 0);
	bool projRes = getLoadedScene()->projectShapeFree(projShape);
	delete projShape;
	if (projRes) { x += amount; }
	else { x += move; }
}



void GameObject::moveY(float amount) {
	GeometryShape* projShape = nullptr;
	float move = 0.0f;
	int sign = (amount > 0) ? 1 : -1;

	while (abs(move + sign) <= abs(amount)) {
		projShape = collider->getShape()->clone();
		projShape->translate(0, move + sign);
		bool projRes = getLoadedScene()->projectShapeFree(projShape);
		if (!projRes) {
			delete projShape;
			y += move;
			return;
		}
		move += sign;
		delete projShape;
	}

	// Final check for the exact amount
	projShape = collider->getShape()->clone();
	projShape->translate(0, amount);
	bool projRes = getLoadedScene()->projectShapeFree(projShape);
	delete projShape;
	if (projRes) { y += amount; }
	else { y += move; }
}
