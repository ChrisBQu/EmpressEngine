#ifndef RENDER_INTERFACE_H
#define RENDER_INTERFACE_H

#include "Sprite.h"
#include "Logger.h"

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

inline const int MAX_QUADS_ON_SCREEN = 1024;

struct OrthographicCamera {
	float zoom;
	glm::ivec2 dimensions;
	glm::ivec2 pos;
};

struct RenderTransform {
	glm::ivec2 atlasOffset;
	glm::ivec2 spriteSize;
	glm::vec2 pos;
	glm::vec2 size;
	float rotation;
};

struct RenderData {
	OrthographicCamera uiCamera;
	OrthographicCamera gameCamera;
	std::vector<RenderTransform> transforms;
};

glm::mat4 makeOrthogrpahicProjectionMatrix(float left, float right, float top, float bottom);

void drawSprite(SpriteID id, glm::vec2 pos, glm::vec2 scale, float rotation = 0.0f);

extern RenderData renderData;

#endif // RENDER_INTERFACE_H