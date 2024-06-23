#include "QuadTree.h"

QuadTree::QuadTree() {
	level = 1;
	bounds = { {0, 0}, {10, 10} };
}

QuadTree::QuadTree(int pLevel, GeometryRectangle pBounds) {
	level = pLevel;
	bounds = pBounds;
}

void QuadTree::clear() {
	objects.clear();
	for (auto& node : nodes) {
		if (node) {
			node->clear();
			node.reset();
		}
	}
	nodes.clear();
}

void QuadTree::split() {
	float subWidth = bounds.size.x / 2.0f;
	float subHeight = bounds.size.y / 2.0f;
	float x = bounds.pos.x;
	float y = bounds.pos.y;

	nodes.emplace_back(std::make_unique<QuadTree>(level + 1, GeometryRectangle{ {x + subWidth, y}, {subWidth, subHeight} }));
	nodes.emplace_back(std::make_unique<QuadTree>(level + 1, GeometryRectangle{ {x, y}, {subWidth, subHeight} }));
	nodes.emplace_back(std::make_unique<QuadTree>(level + 1, GeometryRectangle{ {x, y + subHeight}, {subWidth, subHeight} }));
	nodes.emplace_back(std::make_unique<QuadTree>(level + 1, GeometryRectangle{ {x + subWidth, y + subHeight}, {subWidth, subHeight} }));
}

int QuadTree::getIndex(const GeometryRectangle& aabb) const {
	int index = -1;
	float verticalMidpoint = bounds.pos.x + bounds.size.x / 2.0f;
	float horizontalMidpoint = bounds.pos.y + bounds.size.y / 2.0f;

	bool topQuadrant = (aabb.pos.y < horizontalMidpoint && aabb.pos.y + aabb.size.y < horizontalMidpoint);
	bool bottomQuadrant = (aabb.pos.y > horizontalMidpoint);

	if (aabb.pos.x < verticalMidpoint && aabb.pos.x + aabb.size.x < verticalMidpoint) {
		if (topQuadrant) {
			index = 1;
		}
		else if (bottomQuadrant) {
			index = 2;
		}
	}
	else if (aabb.pos.x > verticalMidpoint) {
		if (topQuadrant) {
			index = 0;
		}
		else if (bottomQuadrant) {
			index = 3;
		}
	}

	return index;
}

void QuadTree::insert(GameObject* obj) {
	if (!nodes.empty()) {
		int index = getIndex(obj->collider->getAABB());

		if (index != -1) {
			nodes[index]->insert(obj);
			return;
		}
	}

	objects.push_back(obj);

	if (objects.size() > MAX_OBJECTS && level < MAX_LEVELS) {
		if (nodes.empty()) {
			split();
		}

		auto it = objects.begin();
		while (it != objects.end()) {
			int index = getIndex((*it)->collider->getAABB());
			if (index != -1) {
				nodes[index]->insert(*it);
				it = objects.erase(it);
			}
			else {
				++it;
			}
		}
	}
}

void QuadTree::retrieve(std::vector<GameObject*>& returnObjects, const GeometryRectangle& aabb) const {
	int index = getIndex(aabb);
	if (index != -1 && !nodes.empty()) {
		nodes[index]->retrieve(returnObjects, aabb);
	}
	returnObjects.insert(returnObjects.end(), objects.begin(), objects.end());
}
