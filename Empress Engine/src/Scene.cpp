#include "Scene.h"
#include "Logger.h"
#include "RenderInterface.h"

#include <algorithm>
#include <limits>
#include <memory>
#include <set>
#include <tuple>
#include <vector>


Scene *CurrentlyLoadedScene;
void loadScene(Scene* s) { CurrentlyLoadedScene = s; }
Scene* getLoadedScene() { return CurrentlyLoadedScene; }

// Constructor
Scene::Scene() { 
	frameCount = 0;
}

void Scene::addObject(GameObject *g) {
	myObjects.push_back(g);
}

void Scene::setCamera(glm::vec2 position, glm::vec2 dimensions) {
	myCamera.pos = position;
	myCamera.dimensions = dimensions;
}

OrthographicCamera Scene::getCamera() {
	return myCamera;
}

void Scene::setTileLayer(int layer, TileLayer tl) {
	tileLayers[layer] = tl;
}

void Scene::toggleTileLayer(int layer, bool toggle) {
	tileLayers[layer].active = toggle;
}

glm::vec2 Scene::getTileLayerPosition(int layer) {
	return tileLayers[layer].myTLD.pos;
}

void Scene::setTileLayerPosition(int layer, glm::vec2 pos) {
	tileLayers[layer].myTLD.pos = pos;
}

void Scene::handleInput(Controller c) {

	for (GameObject* each : myObjects) {
		// Buttons pressed
		if (c.getPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A)) { each->trigger_onPressedA(); }
		if (c.getPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B)) { each->trigger_onPressedB(); }
		if (c.getPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X)) { each->trigger_onPressedX(); }
		if (c.getPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y)) { each->trigger_onPressedY(); }
		if (c.getPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER)) { each->trigger_onPressedL(); }
		if (c.getPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) { each->trigger_onPressedR(); }
		if (c.getPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP)) { each->trigger_onPressedUp(); }
		if (c.getPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN)) { each->trigger_onPressedDown(); }
		if (c.getPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT)) { each->trigger_onPressedLeft(); }
		if (c.getPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) { each->trigger_onPressedRight(); }
		if (c.getPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START)) { each->trigger_onPressedStart(); }
		if (c.getPressed(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_BACK)) { each->trigger_onPressedBack(); }

		// Buttons held down
		if (c.getButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A)) { each->trigger_onHeldA(); }
		if (c.getButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B)) { each->trigger_onHeldB(); }
		if (c.getButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X)) { each->trigger_onHeldX(); }
		if (c.getButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y)) { each->trigger_onHeldY(); }
		if (c.getButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER)) { each->trigger_onHeldL(); }
		if (c.getButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) { each->trigger_onHeldR(); }
		if (c.getButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP)) { each->trigger_onHeldUp(); }
		if (c.getButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN)) { each->trigger_onHeldDown(); }
		if (c.getButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT)) { each->trigger_onHeldLeft(); }
		if (c.getButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) { each->trigger_onHeldRight(); }
		if (c.getButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START)) { each->trigger_onHeldStart(); }
		if (c.getButton(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_BACK)) { each->trigger_onHeldBack(); }

		// Buttons released
		if (c.getReleased(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A)) { each->trigger_onReleasedA(); }
		if (c.getReleased(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B)) { each->trigger_onReleasedB(); }
		if (c.getReleased(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X)) { each->trigger_onReleasedX(); }
		if (c.getReleased(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y)) { each->trigger_onReleasedY(); }
		if (c.getReleased(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER)) { each->trigger_onReleasedL(); }
		if (c.getReleased(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER)) { each->trigger_onReleasedR(); }
		if (c.getReleased(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP)) { each->trigger_onReleasedUp(); }
		if (c.getReleased(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN)) { each->trigger_onReleasedDown(); }
		if (c.getReleased(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT)) { each->trigger_onReleasedLeft(); }
		if (c.getReleased(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT)) { each->trigger_onReleasedRight(); }
		if (c.getReleased(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START)) { each->trigger_onReleasedStart(); }
		if (c.getReleased(SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_BACK)) { each->trigger_onReleasedBack(); }
	}
}

void Scene::update() {
	for (GameObject* each_object : myObjects) {
		each_object->update();
	}
	checkCollisions();
	frameCount++;
}

void Scene::render() {
	for (TileLayer &each_layer : tileLayers) {
		if (each_layer.active) {
			each_layer.render(frameCount);
		}
	}

	for (GameObject* each_object : myObjects) {
		each_object->render();
	}

}

void Scene::deleteObjects() {
	for (GameObject* each : myObjects) {
		delete each;
	}
	myObjects.clear();
}

































class Quadtree {
	static const int MAX_OBJECTS = 5;
	static const int MAX_LEVELS = 5;

	int level;
	std::vector<GameObject*> objects;
	GeometryRectangle bounds;
	std::vector<std::unique_ptr<Quadtree>> nodes;

public:
	Quadtree(int pLevel, GeometryRectangle pBounds) : level(pLevel), bounds(pBounds) {}

	void clear() {
		objects.clear();
		for (auto& node : nodes) {
			if (node) {
				node->clear();
				node.reset();
			}
		}
	}

	void split() {
		float subWidth = bounds.size.x / 2.0f;
		float subHeight = bounds.size.y / 2.0f;
		float x = bounds.pos.x;
		float y = bounds.pos.y;

		nodes.emplace_back(std::make_unique<Quadtree>(level + 1, GeometryRectangle{ {x + subWidth, y}, {subWidth, subHeight} }));
		nodes.emplace_back(std::make_unique<Quadtree>(level + 1, GeometryRectangle{ {x, y}, {subWidth, subHeight} }));
		nodes.emplace_back(std::make_unique<Quadtree>(level + 1, GeometryRectangle{ {x, y + subHeight}, {subWidth, subHeight} }));
		nodes.emplace_back(std::make_unique<Quadtree>(level + 1, GeometryRectangle{ {x + subWidth, y + subHeight}, {subWidth, subHeight} }));
	}

	int getIndex(GameObject* obj) const {
		int index = -1;
		float verticalMidpoint = bounds.pos.x + bounds.size.x / 2.0f;
		float horizontalMidpoint = bounds.pos.y + bounds.size.y / 2.0f;

		GeometryRectangle aabb = obj->collider->getAABB();

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

	void insert(GameObject* obj) {
		if (!nodes.empty()) {
			int index = getIndex(obj);

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
				int index = getIndex(*it);
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

	void retrieve(std::vector<GameObject*>& returnObjects, GameObject* obj) const {
		int index = getIndex(obj);
		if (index != -1 && !nodes.empty()) {
			nodes[index]->retrieve(returnObjects, obj);
		}

		returnObjects.insert(returnObjects.end(), objects.begin(), objects.end());
	}
};


















void Scene::checkCollisions() {
	float x_min = std::numeric_limits<float>::max();
	float x_max = std::numeric_limits<float>::min();
	float y_min = std::numeric_limits<float>::max();
	float y_max = std::numeric_limits<float>::min();

	for (auto& each_object : myObjects) {
		GeometryRectangle aabb = each_object->collider->getAABB();
		x_min = std::min(x_min, aabb.pos.x);
		y_min = std::min(y_min, aabb.pos.y);
		x_max = std::max(x_max, aabb.pos.x + aabb.size.x);
		y_max = std::max(y_max, aabb.pos.y + aabb.size.y);
	}

	GeometryRectangle sceneBounds = { {x_min, y_min}, {x_max - x_min, y_max - y_min} };
	Quadtree quadtree(0, sceneBounds);

	for (auto& each_object : myObjects) {
		quadtree.insert(each_object);
	}

	for (auto& each_object : myObjects) {
		std::vector<GameObject*> possibleCollisions;
		quadtree.retrieve(possibleCollisions, each_object);

		for (auto& other_object : possibleCollisions) {
			if (each_object->id != other_object->id && each_object->collider->collidesWith(other_object->collider)) {
				// LOG("Collision occurred");
			}
		}
	}
}
