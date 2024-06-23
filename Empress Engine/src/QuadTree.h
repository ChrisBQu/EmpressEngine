#ifndef QUAD_TREE_H
#define QUAD_TREE_H

#include "GameObject.h"
#include "Geometry.h"

#include <memory>
#include <vector>

class QuadTree {
	static const int MAX_OBJECTS = 4;
	static const int MAX_LEVELS = 5;

	int level;
	std::vector<GameObject*> objects;
	GeometryRectangle bounds;
	std::vector<std::unique_ptr<QuadTree>> nodes;

public:
	QuadTree();
	QuadTree(int pLevel, GeometryRectangle pBounds);
	void clear();
	void split();
	int getIndex(const GeometryRectangle& aabb) const;
	void insert(GameObject* obj);
	void retrieve(std::vector<GameObject*>& returnObjects, const GeometryRectangle& aabb) const;
};

#endif // QUAD_TREE_H