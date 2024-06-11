#ifndef SPRITE_H
#define SPRITE_H

#include "SpriteAnimation.h"
#include "TransformStruct.h"

class Sprite {
public:
	Sprite();
	SpriteAnimationData animation;
	void tickAnimation();
	void render(TransformStruct T);
};

#endif // SPRITE_H