#include "Scene.h"
#include "Logger.h"
#include "RenderInterface.h"

#include <algorithm>
#include <unordered_set>


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

bool compareGameObjectByMinX(const GameObject* a, const GameObject* b) {
	return a->collider->getAABB().pos.x < b->collider->getAABB().pos.x;
}

bool compareGameObjectByMinY(const GameObject* a, const GameObject* b) {
	return a->collider->getAABB().pos.y < b->collider->getAABB().pos.y;
}

void sweepAndPrune(std::vector<GameObject*>& objects) {
	auto pair_hash = [](const std::pair<uint64_t, uint64_t>& pair) {
		return std::hash<uint64_t>()(pair.first) ^ std::hash<uint64_t>()(pair.second);
		};

	std::unordered_set<std::pair<uint64_t, uint64_t>, decltype(pair_hash)> potentialPairsX(0, pair_hash);

	// Sweep and Prune on the X-Axis
	std::sort(objects.begin(), objects.end(), compareGameObjectByMinX);
	for (size_t i = 0; i < objects.size(); i++) {
		for (size_t j = i + 1; j < objects.size(); j++) {
			if (objects[j]->collider->getAABB().pos.x > objects[i]->collider->getAABB().pos.x + objects[i]->collider->getAABB().size.x) {
				break;
			}
			potentialPairsX.emplace(objects[i]->id, objects[j]->id);
		}
	}

	// Sweep and Prune on the Y-Axis
	std::sort(objects.begin(), objects.end(), compareGameObjectByMinY);
	std::unordered_set<std::pair<uint64_t, uint64_t>, decltype(pair_hash)> potentialPairs(0, pair_hash);
	for (size_t i = 0; i < objects.size(); i++) {
		for (size_t j = i + 1; j < objects.size(); j++) {
			if (objects[j]->collider->getAABB().pos.y > objects[i]->collider->getAABB().pos.y + objects[i]->collider->getAABB().size.y) {
				break;
			}
			auto pair = std::make_pair(objects[i]->id, objects[j]->id);
			if (potentialPairsX.find(pair) != potentialPairsX.end() || potentialPairsX.find(std::make_pair(objects[j]->id, objects[i]->id)) != potentialPairsX.end()) {
				if (objects[i]->solid && objects[j]->solid) {
					potentialPairs.emplace(pair);
				}
			}
		}
	}

	for (const auto& pair : potentialPairs) {
		//LOG("COLLISION: (", pair.first, ") and (", pair.second, ")");
	}
}


void Scene::checkCollisions() {
	sweepAndPrune(myObjects);
}