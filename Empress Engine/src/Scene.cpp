#include "Scene.h"
#include "GameConfig.h"
#include "Logger.h"
#include "RenderInterface.h"

#include <algorithm>
#include <vector>

Scene *CurrentlyLoadedScene;
void loadScene(Scene* s) { CurrentlyLoadedScene = s; }
Scene* getLoadedScene() { return CurrentlyLoadedScene; }

// Constructor
Scene::Scene() {
	frameCount = 0;
}

// Destructor
Scene::~Scene() {
	deleteObjects();
}

void Scene::addObject(GameObject *g) {
	myDynamicObjects.push_back(g);
}

void Scene::addSolid(GeometryShape *s) {
	mySolids.push_back(s);
}

void Scene::addStaticObject(GameObject* g) {
	myStaticObjects.push_back(g);
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
	layers_active[layer] = toggle;
}

glm::vec2 Scene::getTileLayerPosition(int layer) {
	return tileLayers[layer].myTLD.pos;
}

void Scene::setTileLayerPosition(int layer, glm::vec2 pos) {
	tileLayers[layer].myTLD.pos = pos;
}

void Scene::handleInput(Controller c) {

	for (GameObject* each : myDynamicObjects) {
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
	for (GameObject* each_object : myDynamicObjects) {
		each_object->update();
	}
	for (GameObject* each_object : myStaticObjects) {
		each_object->update();
	}
	frameCount++;
}

void Scene::render() {

	if (layers_active[0]) { tileLayers[0].render(frameCount); }
	if (layers_active[0]) { tileLayers[1].render(frameCount); }
	if (layers_active[0]) { tileLayers[2].render(frameCount); }
	if (layers_active[0]) { tileLayers[3].render(frameCount); }

	for (GameObject* each_object : myStaticObjects) {
		each_object->render();
	}

	for (GameObject* each_object : myDynamicObjects) {
		each_object->render();
	}

	if (DRAW_BOUNDING_BOXES) {
		for (GeometryShape *each_shape : mySolids) {
			std::vector<GeometryLineSegment> lines = getLineSegmentsOfShape(*each_shape);
			for (GeometryLineSegment each_line : lines) {
				drawLine(pointToVec(each_line.start), pointToVec(each_line.end), 2, { 0.0, 1.0, 0.0, 1.0 });
			}
		}
	}

}

void Scene::deleteObjects() {
	for (GameObject* each : myDynamicObjects) {
		delete each;
	}
	myDynamicObjects.clear();
	for (GameObject* each : myStaticObjects) {
		delete each;
	}
	myStaticObjects.clear();
	for (GeometryShape* each : mySolids) {
		delete each;
	}
	mySolids.clear();
}

std::vector<GameObject*> Scene::queryCollisions(GameObject* requester) {

	std::vector<GameObject*> ret_vect;

	for (GameObject* each_object : myDynamicObjects) {
		if (requester->id != each_object->id) {
			if (requester->collider->collidesWith(each_object->collider)) {
				ret_vect.push_back(each_object);
			}
		}
	}

	return ret_vect;

}

float Scene::fireRay(GeometryRay r) {
	float distance = 99999;
	for (GeometryShape *each_shape: mySolids) {
		std::vector<GeometryPoint> gps = geometryGetIntersections(r, *each_shape);
		for (GeometryPoint& ep : gps) {
			float new_distance = distanceBetweenPoints(ep, r.start);
			if (distance > new_distance) { distance = new_distance; }
		}
	}
	return distance;
}

bool Scene::projectShapeFree(GeometryShape* s) {
	for (GeometryShape* each_shape : mySolids) {
		if (geometryGetIntersections(*each_shape, *s).size() > 0) {
			return false;  // There is an intersection, so the shape is not free
		}
	}
	return true;  // No intersections found, so the shape is free
}
