#include "SceneBuilder.h"
#include "GameObjectTypes.h"
#include "Logger.h"
#include "Tileset.h"
#include "Utility.h"

#include "3rdParty/json.hpp"

Scene* buildSceneFromJSON(const char* filepath) {
	std::string s = readFileIntoString(filepath);
	if (s.length() == 0) {
		LOG_ERROR("Could not build scene because JSON file could not be read: ", filepath);
		return nullptr;
	}
	if (!nlohmann::json::accept(s)) {
		LOG_ERROR("Could not build scene because JSON is malformed: ", filepath);
		return nullptr;
	}
	nlohmann::json jsondata = nlohmann::json::parse(s);

	Scene* newscene = new Scene();

	// Set camera
	nlohmann::json j_camera = jsondata["camera"];
	newscene->setCamera({ j_camera["x"], j_camera["y"] }, { j_camera["width"], j_camera["height"] });

	// Create objects
	try {
		nlohmann::json j_objects = jsondata["objects"];
		for (auto it = j_objects.begin(); it != j_objects.end(); it++) {
			int x = it.value()["x"];
			int y = it.value()["y"];
			GameObjectType got = stringToGameObjectType(it.value()["type"]);
			GameObject* newobj = generateGameObject(got, x, y);
			if (newobj != nullptr) { newscene->addObject(newobj); }
		}
	}
	catch(int e) { LOG_ERROR("<objects> section of Scene JSON malformed for: ", filepath); }

	// Create solid geometry
	try {
		nlohmann::json j_geometry = jsondata["geometry"];
		for (auto it = j_geometry.begin(); it != j_geometry.end(); it++) {
			std::string geo_type = it.value()["type"];

			if (geo_type == "Rectangle") {
				float x = it.value()["x"];
				float y = it.value()["y"];
				float w = it.value()["width"];
				float h = it.value()["height"];
				GeometryRectangle* r = new GeometryRectangle({ { x, y }, { w, h } });
				newscene->addSolid(r);
			}

			if (geo_type == "Circle") {
				float x = it.value()["x"];
				float y = it.value()["y"];
				float r = it.value()["radius"];
				GeometryCircle* c = new GeometryCircle({ { x, y }, r });
				newscene->addSolid(c);
			}

			if (geo_type == "Triangle") {
				float x1 = it.value()["x1"];
				float y1 = it.value()["y1"];
				float x2 = it.value()["x2"];
				float y2 = it.value()["y2"];
				float x3 = it.value()["x3"];
				float y3 = it.value()["y3"];
				GeometryTriangle* tri = new GeometryTriangle({ x1,y1 }, { x2, y2 }, { x3, y3 });
				newscene->addSolid(tri);
			}
		}
	}
	catch (int e) { LOG_ERROR("<geometry> section of Scene JSON malformed for: ", filepath); }

	// Turn on appropriate tile layers, and set correct tileset
	try {
		nlohmann::json j_tilelayers = jsondata["tile_layers_active"];
		if (j_tilelayers["0"] == 1) { newscene->toggleTileLayer(0, true); }
		if (j_tilelayers["1"] == 1) { newscene->toggleTileLayer(1, true); }
		if (j_tilelayers["2"] == 1) { newscene->toggleTileLayer(2, true); }
		if (j_tilelayers["3"] == 1) { newscene->toggleTileLayer(3, true); }
	}
	catch (int e) { LOG_ERROR("<tile_layers_active> section of Scene JSON malformed for: ", filepath);  }


	std::vector<unsigned int> someTiles;

	// TESTING
	for (int i = 0; i < 1000000; i++) {
		someTiles.push_back(rand() % 4);
	}


	nlohmann::json j_tilesets = jsondata["tilesets"];

	try {
		nlohmann::json j_tiles_0 = jsondata["tiles0"];
		nlohmann::json tileset_layer_data_0 = jsondata["tileset_layer_data_0"];
		TilesetLayerData tld0 = { tileset_layer_data_0[0], tileset_layer_data_0[1], {tileset_layer_data_0[2], tileset_layer_data_0[3] }, someTiles };
		if (j_tilesets["0"] != "") {
			TilesetData tsd0 = getTilesetData(j_tilesets["0"]);
			TileLayer TL0(tsd0, tld0);
			newscene->setTileLayer(0, TL0);
		}
	}
	catch (int e) { LOG_ERROR("Error building tileset0 from Scene JSON for: ", filepath); }	

	try {
		nlohmann::json j_tiles_1 = jsondata["tiles1"];
		nlohmann::json tileset_layer_data_1 = jsondata["tileset_layer_data_1"];
		TilesetLayerData tld1 = { tileset_layer_data_1[0], tileset_layer_data_1[1], {tileset_layer_data_1[2], tileset_layer_data_1[3] }, someTiles };
		if (j_tilesets["1"] != "") {
			TilesetData tsd1 = getTilesetData(j_tilesets["1"]);
			TileLayer TL1(tsd1, tld1);
			newscene->setTileLayer(1, TL1);
		}
	}
	catch (int e) { LOG_ERROR("Error building tileset1 from Scene JSON for: ", filepath); }

	return newscene;

}