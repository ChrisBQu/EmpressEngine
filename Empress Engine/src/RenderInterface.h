#ifndef RENDER_INTERFACE_H
#define RENDER_INTERFACE_H

#include "Logger.h"

#include <map>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

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
	float depth;
	float rotation;
	float alpha;

	// To handle alignment in the GPU
	float alignment_padding;
};

struct TileTransform {
	glm::vec2 pos;
	glm::vec2 size;
	glm::vec2 tileSize;
};

struct RenderData {
	OrthographicCamera uiCamera;
	OrthographicCamera gameCamera;
	std::map<std::string, std::vector<RenderTransform>> transforms;
};

glm::mat4 makeOrthographicProjectionMatrix(float left, float right, float top, float bottom);

void drawQuad(std::string tex_identifier, glm::vec2 tex_pos, glm::vec2 tex_size, glm::vec2 pos, glm::vec2 scale, int depth = 0, float rotation = 0.0f, float alpha = 1.0f);

extern RenderData renderData;
extern std::vector<TileTransform> renderTileData;

#endif // RENDER_INTERFACE_H