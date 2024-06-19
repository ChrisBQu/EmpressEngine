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

void Scene::addObject(GameObject g) {
	myObjects.push_back(g);
}

void Scene::setTileLayer(int layer, TilesetLayerData tld) {
	tileLayers[layer].data = tld;
}

void Scene::toggleTileLayer(int layer, bool toggle) {
	tileLayers[layer].active = toggle;
}


void Scene::update() {
	for (GameObject& each_object : myObjects) {
		each_object.update();
	}
	frameCount++;
}

void Scene::render() {
	for (GameObject &each_object : myObjects) {
		each_object.render();
	}

	for (TileLayer &each_layer : tileLayers) {
		if (each_layer.active) {
			drawTileset(each_layer.data.tilesetData, each_layer.data.tileData, each_layer.data.pos, each_layer.data.tilesPerRow, frameCount, each_layer.data.depth);
		}
	}

}