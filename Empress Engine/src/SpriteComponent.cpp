#include "SpriteComponent.h"
#include "GameObject.h"
#include "Logger.h"
#include "RenderInterface.h"
#include "Scene.h"

// Constructor
SpriteComponent::SpriteComponent(GameObject *p) {
	parent = p;
	animation = { false, "_null_", {0, 0}, {0, 0}, 0, 0, 0, { 0 }, 0 };
}

// Set the animation data by looking up the data associated with a string identifier
void SpriteComponent::setAnimation(std::string identifier) {
	animation = getSpriteAnimationData(identifier);
}

// Create an "animation" that is actually just a static, still sprite from a given texture atlas
void SpriteComponent::setStaticSprite(std::string atlasIdentifier, glm::ivec2 atlasOffset, glm::ivec2 atlasSize) {
	// TO DO: Figure out why the atlasOffset.y must subtract atlasSize.y
	animation = { true, atlasIdentifier, {atlasOffset[0], atlasOffset[1] - atlasSize[1]}, atlasSize, 1, 1, 1, { 1 }, 1 };
}


// Draw the sprite to the screen using its internal animation data, and a Transformation T
void SpriteComponent::render() {
	OrthographicCamera cam = getLoadedScene()->getCamera();
	float cameraLeft = cam.pos.x - cam.dimensions.x / 2.0f;
	float cameraRight = cam.pos.x + cam.dimensions.x / 2.0f;
	float cameraBottom = cam.pos.y - cam.dimensions.y / 2.0f;
	float cameraTop = cam.pos.y + cam.dimensions.y / 2.0f;

	float spriteLeft = parent->x - animation.atlasSize.x / 2.0f;
	float spriteRight = parent->x + animation.atlasSize.x / 2.0f;
	float spriteBottom = parent->y - animation.atlasSize.y / 2.0f;
	float spriteTop = parent->y + animation.atlasSize.y / 2.0f;

	// Check if the tile is within the camera view
	if (spriteRight > cameraLeft && spriteLeft < cameraRight + animation.atlasSize.x && spriteTop > cameraBottom && spriteBottom < cameraTop + animation.atlasSize.y) {
		if (animation.framesPerRow > 0) {
			int atlasRow = animation.currentFrame / animation.framesPerRow;
			int atlasCol = animation.currentFrame % animation.framesPerRow;
			int atlasX = animation.atlasOffset[0] + (animation.atlasSize[0] * atlasCol);
			int atlasY = animation.atlasOffset[1] + (animation.atlasSize[1] * atlasRow);
			glm::vec2 renderSize = { parent->transform->size[0] * parent->scale[0], parent->transform->size[1] * parent->scale[1] };
			drawQuad(animation.atlasIdentifier, { atlasX, atlasY }, animation.atlasSize, {parent->x, parent->y }, renderSize, parent->transform->depth, parent->transform->rotation, parent->alpha);
		}
	}
}

// Call once per frame to continue progressing through sprite's animation
void SpriteComponent::tickAnimation() {
	if (animation.paused) { return; }
	if (animation.frameCount > 0) {
		if (animation.ticksOnFrame++ == animation.frameTimes[animation.currentFrame]) {
			animation.ticksOnFrame = 0;
			animation.currentFrame = (animation.currentFrame + 1) % animation.frameCount;
		}
	}
}