#ifndef RENDER_INTERFACE_H
#define RENDER_INTERFACE_H

#include "Logger.h"
#include "Tileset.h"

#include <map>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

struct OrthographicCamera {
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

struct TextRenderItem {
	std::string text;
	std::string fontIdentifier;
	glm::vec2 pos;
	glm::vec4 color;
	glm::vec2 scale;
};

struct RenderData {
	OrthographicCamera gameCamera;
	OrthographicCamera uiCamera;
	std::vector<TextRenderItem> textItems;
	std::map<std::string, std::vector<RenderTransform>> transforms;
};

void drawQuad(std::string tex_identifier, glm::vec2 tex_pos, glm::vec2 tex_size, glm::vec2 pos, glm::vec2 scale, int depth = 0, float rotation = 0.0f, float alpha = 1.0f);
void drawTileset(TilesetData tsd, std::vector<unsigned int> &tile_indices, glm::vec2 offset_pos, unsigned int tiles_per_row, unsigned int framecount, unsigned int depth);
void drawText(std::string text, std::string font_identifier, glm::vec2 pos, glm::vec2 scale, glm::vec4 color);

extern RenderData renderData;

#endif // RENDER_INTERFACE_H