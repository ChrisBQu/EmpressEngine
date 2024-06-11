#ifndef SPRITE_H
#define SPRITE_H

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

class Sprite {
public:
	Sprite();
	glm::ivec2 pos;
	glm::ivec2 size;
	glm::ivec2 scale;
	int depth;
	float rotation;
	float alpha;
	SpriteAnimationData animation;
	void tickAnimation();
	void render();
};

#endif // SPRITE_H