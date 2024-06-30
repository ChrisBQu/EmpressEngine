#ifndef RENDER_INTERFACE_H
#define RENDER_INTERFACE_H

#include "Logger.h"
#include "Tileset.h"

#include <map>
#include <string>
#include <vector>

#include <GL/glew.h>

constexpr int RENDERING_DEPTH_LAYERS = 400;

struct RenderTransform {
	glm::ivec2 atlasOffset;
	glm::ivec2 spriteSize;
	glm::vec2 pos;
	glm::vec2 size;
	float rotation;
	float alpha;
};

struct TextRenderItem {
	std::string text;
	std::string fontIdentifier;
	glm::vec2 pos;
	glm::vec4 color;
	glm::vec2 scale;
};

struct PrimitiveQuadRenderItem {
	glm::vec2 point1;
	glm::vec2 point2;
	glm::vec2 point3;
	glm::vec2 point4;
	glm::vec4 color;
};

struct RenderData {
	std::vector<TextRenderItem> textItems;
	std::vector<PrimitiveQuadRenderItem> primitiveQuadItems;
	std::map<std::string, std::vector<RenderTransform>> transforms[RENDERING_DEPTH_LAYERS + 1];
	std::map<std::string, std::vector<RenderTransform>> uitransforms[RENDERING_DEPTH_LAYERS + 1];
};

void drawQuad(std::string tex_identifier, glm::vec2 tex_pos, glm::vec2 tex_size, glm::vec2 pos, glm::vec2 size, int depth = 0, float rotation = 0.0f, float alpha = 1.0f);
void drawUIQuad(std::string tex_identifier, glm::vec2 tex_pos, glm::vec2 tex_size, glm::vec2 pos, glm::vec2 scale, int depth = 0, float rotation = 0.0f, float alpha = 1.0f);
void drawText(std::string text, std::string font_identifier, glm::vec2 pos, glm::vec2 scale, glm::vec4 color);
void drawLine(glm::vec2 start, glm::vec2 end, float thickness, glm::vec4 color);

extern RenderData renderData;

#endif // RENDER_INTERFACE_H