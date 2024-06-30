#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include "SpriteAnimation.h"

// Forward declaration
class GameObject;

class SpriteComponent {
public:
	SpriteComponent(GameObject *p);
	void setAnimation(std::string identifier);
	void setStaticSprite(std::string atlasIdentifier, glm::ivec2 atlasOffset, glm::ivec2 atlasSize);
	void tickAnimation();
	void render();
	float getWidth();
	float getHeight();
private:
	GameObject* parent;
	SpriteAnimationData animation;
};

#endif // SPRITE_COMPONENT_H