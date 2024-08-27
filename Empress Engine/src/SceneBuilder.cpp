#include "SceneBuilder.h"
#include "GameObjectTypes.h"
#include "Logger.h"
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

	nlohmann::json j_camera = jsondata["camera"];
	newscene->setCamera({ j_camera["x"], j_camera["y"] }, { j_camera["width"], j_camera["height"] });

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
				//newscene->addSolid(c);
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

			if (geo_type == "LINE") {
				float x1 = it.value()["x1"];
				float y1 = it.value()["y1"];
				float x2 = it.value()["x2"];
				float y2 = it.value()["y2"];
				GeometryLineSegment line = new GeometryLineSegment({ x1, y1 }, { x2, y2 });
				newscene->addSolid(line);
			}

		}
	}
	catch (int e) { LOG_ERROR("<geometry> section of Scene JSON malformed for: ", filepath); }


	return newscene;

}