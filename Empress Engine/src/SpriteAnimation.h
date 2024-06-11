#ifndef SPRITE_ANIMATION_H
#define SPRITE_ANIMATION_H

#include <string>
#include <vector>
#include <glm/glm.hpp>

struct SpriteAnimationData {
	bool paused;
	std::string atlasIdentifier;
	glm::ivec2 atlasOffset;
	glm::ivec2 atlasSize;
	unsigned int currentFrame;
	unsigned int frameCount;
	unsigned int ticksOnFrame;
	std::vector<unsigned int> frameTimes;
	unsigned int framesPerRow;
};

void buildSpriteAnimationList(const char* filepath);
SpriteAnimationData getSpriteAnimationData(std::string identifier);

#endif // SPRITE_ANIMATION_H