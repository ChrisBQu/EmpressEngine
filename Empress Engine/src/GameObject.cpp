#include "GameObject.h"


GameObject::GameObject() {
	visible = true;
	transform.pos = { 0, 0 };
	transform.scale = { 1.0, 1.0 };
	transform.depth = 0;
	transform.rotation = 0;
	transform.alpha = 1.0;
}


void GameObject::update() {
	if (visible) {
		sprite.tickAnimation();
	}
}

void GameObject::render() {
	if (visible) {
		sprite.render(transform);
	}
}