#include "SceneBuilder.h"
#include "GameObjectTypes.h"
#include "Logger.h"
#include "Scene.h"
#include "Utility.h"

#include "3rdParty/json.hpp"

void buildSceneFromJSON(const char* filepath) {
	std::string s = readFileIntoString(filepath);
	if (s.length() == 0) {
		LOG_ERROR("Could not build scene because JSON file could not be read: ", filepath);
		return;
	}
	if (!nlohmann::json::accept(s)) {
		LOG_ERROR("Could not build scene because JSON is malformed: ", filepath);
		return;
	}
	nlohmann::json jsondata = nlohmann::json::parse(s);

	Scene* newscene = new Scene();

	nlohmann::json j_camera = jsondata["camera"];
	newscene->setCamera({ j_camera["x"], j_camera["y"] }, { j_camera["width"], j_camera["height"] });



	nlohmann::json j_objects = jsondata["objects"];
	for (auto it = j_objects.begin(); it != j_objects.end(); it++) {
		int x = it.value()["x"];
		int y = it.value()["y"];
		GameObjectType got = stringToGameObjectType(it.value()["type"]);
		LOG("got: ", (int)got);
		GameObject* newobj = generateGameObject(got, x, y);
		if (newobj != nullptr) { newscene->addObject(newobj); }
	}

	loadScene(newscene);


}