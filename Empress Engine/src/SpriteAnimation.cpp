#include "SpriteAnimation.h"

#include "Logger.h"
#include "Utility.h"

#include <map>
#include "3rdParty/json.hpp"

std::map<std::string, SpriteAnimationData> SpriteAnimationList;

// Call once to load a JSON file into the SpriteAnimationList map, for use.
// The JSON object should be in the following format:
// {
//	"ANIMATION_NAME" : {
//		"atlasIdentifier" : "texture_atlas_identifier",
//		"atlasOffset" : [0, 0] ,
//		"atlasSize" : [32, 32] ,
//		"frameCount" : 4,
//		"framesPerRow" : 2,
//		"frameTimes" : [12, 12, 12, 12]
//	  }
// }
void buildSpriteAnimationList(const char* filepath) {
	std::string s = readFileIntoString(filepath);
	if (s.length() == 0) {
		LOG_ERROR("Could not populate SpriteAnimationData object because JSON file could not be read.");
		return;
	}
	if (!nlohmann::json::accept(s)) {
		LOG_ERROR("Could not populate SpriteAnimationData object because JSON is malformed.");
		return;
	}
	nlohmann::json jsondata = nlohmann::json::parse(s);
	for (auto it = jsondata.begin(); it != jsondata.end(); it++) {
		SpriteAnimationData newdata;
		newdata.paused = false;
		newdata.currentFrame = 0;
		newdata.ticksOnFrame = 0;
		try {
			newdata.atlasIdentifier = it.value()["atlasIdentifier"];
			newdata.atlasOffset = { it.value()["atlasOffset"][0], it.value()["atlasOffset"][1] };
			newdata.atlasSize = { it.value()["atlasSize"][0], it.value()["atlasSize"][1] };
			newdata.frameCount = it.value()["frameCount"];
			newdata.framesPerRow = it.value()["framesPerRow"];
			newdata.frameTimes = it.value()["frameTimes"].get<std::vector<unsigned int>>();
			SpriteAnimationList[it.key()] = newdata;
		}
		catch (nlohmann::json::exception& e) {
			LOG_ERROR("Sprite Animation Data JSON for (", it.key(), ") contained inappropriate data types. Skipping loading.");
		}
	}
}

SpriteAnimationData getSpriteAnimationData(std::string identifier) {
	if (SpriteAnimationList.find(identifier) == SpriteAnimationList.end()) {
		LOG_ERROR("Requested sprite animation data that doesn't exist: ", identifier);
		SpriteAnimationData blankdata = { true, "_null_", {0, 0}, {0, 0}, 0, 0, 0, {0}, 0 };
		return blankdata;
	}
	return SpriteAnimationList[identifier];
}