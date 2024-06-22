#ifndef SCENE_H
#define SCENE_H

#include "GameObject.h"
#include "Controller.h"
#include "Engine.h"
#include "OrthographicCamera.h"
#include "Tileset.h"

#include <vector>
#include <glm/glm.hpp>

constexpr int NUMBER_OF_TILE_LAYERS = 4;

class Scene {
public:
	Scene();
	void setTileLayer(int layer, TileLayer tl);
	void toggleTileLayer(int layer, bool toggle);

	void addObject(GameObject* g);
	void deleteObjects();

	void setCamera(glm::vec2 position, glm::vec2 dimensions);
	OrthographicCamera getCamera();

	glm::vec2 getTileLayerPosition(int layer);
	void setTileLayerPosition(int layer, glm::vec2 pos);

	friend class Engine;

private:

	OrthographicCamera myCamera;

	void handleInput(Controller c);
	void checkCollisions();
	void update();
	void render();

	unsigned int frameCount;
	TileLayer tileLayers[NUMBER_OF_TILE_LAYERS];
	std::vector<GameObject*> myObjects;
};

void loadScene(Scene* s);
Scene* getLoadedScene();

#endif