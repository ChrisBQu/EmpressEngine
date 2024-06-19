#include "GameObject.h"
#include "Logger.h"

GameObject::GameObject() {
	visible = true;
	transform.pos = { 0, 0 };
	transform.scale = { 1.0, 1.0 };
	transform.depth = 0;
	transform.rotation = 0;
	transform.alpha = 1.0;
}

void GameObject::update() {
}

void GameObject::render() {
	sprite.tickAnimation();
	if (visible) {
		sprite.render(transform);
	}
}