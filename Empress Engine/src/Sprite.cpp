#include "Sprite.h"
#include "RenderInterface.h"

// Constructor
Sprite::Sprite() {
	animation = { false, "_null_", {0, 0}, {0, 0}, 0, 0, 0, { 0 }, 0 };
}

// Draw the sprite to the screen using its internal animation data, and a Transformation T
void Sprite::render(TransformStruct T) {
	if (animation.framesPerRow > 0) {
		int atlasRow = animation.currentFrame / animation.framesPerRow;
		int atlasCol = animation.currentFrame % animation.framesPerRow;
		int atlasX = animation.atlasOffset[0] + (animation.atlasSize[0] * atlasCol);
		int atlasY = animation.atlasOffset[1] + (animation.atlasSize[1] * atlasRow);
		drawQuad({ atlasX, atlasY }, animation.atlasSize, T.pos, T.scale, T.depth, T.rotation, T.alpha);
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