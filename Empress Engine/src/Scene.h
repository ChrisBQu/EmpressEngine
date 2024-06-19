#ifndef SCENE_H
#define SCENE_H

#include "GameObject.h"
#include "Tileset.h"

#include <vector>
#include <glm/glm.hpp>

constexpr int NUMBER_OF_TILE_LAYERS = 4;

struct TileLayer {
	bool active;
	TilesetLayerData data;
};

class Scene {
public:
	Scene();
	void addObject(GameObject *g);
	void setTileLayer(int layer, TilesetLayerData tld);
	void toggleTileLayer(int layer, bool toggle);
	void update();
	void render();
private:
	unsigned int frameCount;
	TileLayer tileLayers[NUMBER_OF_TILE_LAYERS];
	std::vector<GameObject*> myObjects;
};

#endif