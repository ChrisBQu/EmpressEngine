#include "Sprite.h"

Sprite getSprite(SpriteID id) {
	Sprite sprite = {};
	switch (id) 
	{
		case SPRITE_STAR:
		{
			sprite.atlasOffset = { 0, 0 };
			sprite.spriteSize = { 32, 32 };
		}
	}
	return sprite;
}