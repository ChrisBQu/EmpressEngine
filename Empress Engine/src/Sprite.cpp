#include "Sprite.h"
#include "Logger.h"
#include "RenderInterface.h"

// Constructor
Sprite::Sprite() {
	animation = { false, "_null_", {0, 0}, {0, 0}, 0, 0, 0, { 0 }, 0 };
}

void Sprite::setAnimation(std::string identifier) {
	animation = getSpriteAnimationData(identifier);
}

void Sprite::setStaticSprite(std::string atlasIdentifier, glm::ivec2 atlasOffset, glm::ivec2 atlasSize) {
	// TO DO: Figure out why the atlasOffset.y must subtract atlasSize.y
	animation = { true, atlasIdentifier, {atlasOffset[0], atlasOffset[1] - atlasSize[1]}, atlasSize, 1, 1, 1, { 1 }, 1 };
}


// Draw the sprite to the screen using its internal animation data, and a Transformation T
void Sprite::render(TransformStruct T) {
	if (animation.framesPerRow > 0) {
		int atlasRow = animation.currentFrame / animation.framesPerRow;
		int atlasCol = animation.currentFrame % animation.framesPerRow;
		int atlasX = animation.atlasOffset[0] + (animation.atlasSize[0] * atlasCol);
		int atlasY = animation.atlasOffset[1] + (animation.atlasSize[1] * atlasRow);
		drawQuad(animation.atlasIdentifier, { atlasX, atlasY }, animation.atlasSize, T.pos, T.scale, T.depth, T.rotation, T.alpha);
	}
}

// Call once per frame to continue progressing through sprite's animation
void Sprite::tickAnimation() {
	if (animation.paused) { return; }
	if (animation.frameCount > 0) {
		if (animation.ticksOnFrame++ == animation.frameTimes[animation.currentFrame]) {
			animation.ticksOnFrame = 0;
			animation.currentFrame = (animation.currentFrame + 1) % animation.frameCount;
		}
	}
}