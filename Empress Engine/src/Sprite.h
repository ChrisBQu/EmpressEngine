#ifndef SPRITE_H
#define SPRITE_H

#include "SpriteAnimation.h"
#include "TransformStruct.h"

class Sprite {
public:
	Sprite();
	void setAnimation(std::string identifier);
	void setStaticSprite(std::string atlasIdentifier, glm::ivec2 atlasOffset, glm::ivec2 atlasSize);
	void tickAnimation();
	void render(TransformStruct T);
private:
	SpriteAnimationData animation;
};

#endif // SPRITE_H