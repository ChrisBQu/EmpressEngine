#ifndef SPRITE_H
#define SPRITE_H

#include <glm/glm.hpp>


enum SpriteID {
	SPRITE_STAR,
	SPRITE_COUNT
};

struct Sprite {
	glm::ivec2 atlasOffset;
	glm::ivec2 spriteSize;
};

Sprite getSprite(SpriteID id);

#endif // SPRITE_H