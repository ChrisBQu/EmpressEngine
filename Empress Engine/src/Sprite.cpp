#include "Sprite.h"
#include "Logger.h"
#include "RenderInterface.h"
#include "Utility.h"

#include <map>
#include "3rdParty/json.hpp"

std::map<std::string, SpriteAnimationData> SpriteAnimationList;

void buildSpriteAnimationList() {
	std::string s = readFileIntoString("assets/animation_data.json");
	if (s.length() == 0) {
		LOG_ERROR("Could not populate SpriteAnimationData object because JSON file could not be read.");
		return;
	}
	if (!nlohmann::json::accept(s)) {
		LOG_ERROR("Could not populate SpriteAnimationData object because JSON is malformed.");
		return;
	}
	nlohmann::json jsondata = nlohmann::json::parse(s);

	// TODO: Error checking on the format of the JSON
	for (auto it = jsondata.begin(); it != jsondata.end(); it++) {
		SpriteAnimationData newdata;
		newdata.paused = false;
		newdata.currentFrame = 0;
		newdata.ticksOnFrame = 0;
		newdata.atlasIdentifier = it.value()["atlasIdentifier"];
		newdata.atlasOffset = { it.value()["atlasOffset"][0], it.value()["atlasOffset"][1] };
		newdata.atlasSize = { it.value()["atlasSize"][0], it.value()["atlasSize"][1] };
		newdata.frameCount = it.value()["frameCount"];
		newdata.framesPerRow = it.value()["framesPerRow"];
		newdata.frameTimes = it.value()["frameTimes"].get<std::vector<unsigned int>>();
		SpriteAnimationList[it.key()] = newdata;
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

Sprite::Sprite() {}

void Sprite::render() {
	if (animation.framesPerRow > 0) {
		int atlasRow = animation.currentFrame / animation.framesPerRow;
		int atlasCol = animation.currentFrame % animation.framesPerRow;
		int atlasX = animation.atlasOffset[0] + (animation.atlasSize[0] * atlasCol);
		int atlasY = animation.atlasOffset[1] + (animation.atlasSize[1] * atlasRow);
		drawQuad({ atlasX, atlasY }, animation.atlasSize, pos, scale, depth, rotation);
	}
}

void Sprite::tickAnimation() {
	if (animation.paused) { return; }
	if (animation.frameCount > 0) {
		if (animation.ticksOnFrame++ == animation.frameTimes[animation.currentFrame]) {
			animation.ticksOnFrame = 0;
			animation.currentFrame = (animation.currentFrame + 1) % animation.frameCount;
		}
	}
}