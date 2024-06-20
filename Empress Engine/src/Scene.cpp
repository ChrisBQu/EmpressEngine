#include "Scene.h"
#include "RenderInterface.h"


// Constructor
Scene::Scene() { 
	frameCount = 0;
	for (unsigned int i = 0; i < NUMBER_OF_TILE_LAYERS; i++) {
		tileLayers[i].active = false;
		tileLayers[i].data.tilesPerRow = 1;
		tileLayers[i].data.tilesetData = {
		"_null_", { 16, 16 }, 1,
		{
			{0, {0, 1}},
		}
	};
	}
}

void Scene::addObject(GameObject *g) {
	myObjects.push_back(g);
}

void Scene::setTileLayer(int layer, TilesetLayerData tld) {
	tileLayers[layer].data = tld;
}

void Scene::toggleTileLayer(int layer, bool toggle) {
	tileLayers[layer].active = toggle;
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
	frameCount++;
}

void Scene::render() {
	for (GameObject* each_object : myObjects) {
		each_object->render();
	}
	for (TileLayer &each_layer : tileLayers) {
		if (each_layer.active) {
			drawTileset(each_layer.data.tilesetData, each_layer.data.tileData, each_layer.data.pos, each_layer.data.tilesPerRow, frameCount, each_layer.data.depth);
		}
	}
}

void Scene::deleteObjects() {
	for (GameObject* each : myObjects) {
		delete each;
	}
	myObjects.clear();
}