#include "Logger.h"
#include "RenderInterface.h"
#include "Sprite.h"

#include <cmath>
#include <glm/gtc/matrix_transform.hpp>


RenderData renderData;

void drawSprite(SpriteID id, glm::vec2 pos, glm::vec2 scale, int depth, float rotation) {
	Sprite sprite = getSprite(id);
	RenderTransform transform;
	transform.pos = pos - glm::vec2(sprite.spriteSize) / 2.0f;
	transform.size = glm::vec2(sprite.spriteSize.x * scale.x, sprite.spriteSize.y * scale.y);
	transform.atlasOffset = sprite.atlasOffset;
	transform.spriteSize = sprite.spriteSize;
    transform.depth = -1.0 / (1.0 + std::exp(-depth)); // Map an integer to a float in range [0, 1]
    transform.rotation = rotation;
	renderData.transforms.push_back(transform);}

glm::mat4 makeOrthogrpahicProjectionMatrix(float left, float right, float top, float bottom) {
    glm::mat4 result(1.0f); 
    result[0][0] = 2.0f / (right - left);
    result[1][1] = 2.0f / (top - bottom);
    result[2][2] = 1.0f / (1.0f - 0.0f);
    result[3][0] = -(right + left) / (right - left);
    result[3][1] = (top + bottom) / (top - bottom);
    result[3][2] = 0.0f;
    result[3][3] = 1.0f;
    return result;
}