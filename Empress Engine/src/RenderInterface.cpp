#include "FontManager.h"
#include "GameConfig.h"
#include "Logger.h"
#include "RenderInterface.h"
#include "Scene.h"
#include "Utility.h"
#include <cmath>

RenderData renderData;

void drawQuad(std::string tex_identifier, glm::vec2 tex_pos, glm::vec2 tex_size, glm::vec2 pos, glm::vec2 scale, int depth, float rotation, float alpha) {
    RenderTransform transform;
    transform.pos = pos - glm::vec2(tex_size) / 2.0f;
    transform.size = glm::vec2(tex_size[0] * scale.x, tex_size[1] * scale.y);
    transform.atlasOffset = tex_pos;
    transform.spriteSize = tex_size;
    float minVal = -200.0;
    float maxVal = 200.0;
    float normalizedDepth = (float)((depth - minVal) / (maxVal - minVal));
    transform.depth = normalizedDepth;
    transform.rotation = rotation;
    transform.alpha = alpha;
    renderData.transforms[tex_identifier].push_back(transform);
}

void drawUIQuad(std::string tex_identifier, glm::vec2 tex_pos, glm::vec2 tex_size, glm::vec2 pos, glm::vec2 scale, int depth, float rotation, float alpha) {
    float xs = (float)gameConfig.screenWidth / (float)DEFAULT_SCREEN_WIDTH;
    float ys = (float)gameConfig.screenHeight / (float)DEFAULT_SCREEN_HEIGHT;
    RenderTransform transform;
    transform.pos = pos;
    transform.pos = { transform.pos[0] * xs, transform.pos[1] * ys };
    transform.size = glm::vec2(tex_size[0] * scale.x * xs, tex_size[1] * scale.y * ys);
    transform.atlasOffset = tex_pos;
    transform.spriteSize = tex_size;
    float minVal = -200.0;
    float maxVal = 200.0;
    float normalizedDepth = (float)((depth - minVal) / (maxVal - minVal));
    transform.depth = normalizedDepth;
    transform.rotation = rotation;
    transform.alpha = alpha;
    renderData.uitransforms[tex_identifier].push_back(transform);
}

void drawText(std::string text, std::string font_identifier, glm::vec2 pos, glm::vec2 scale, glm::vec4 color) {
    TextRenderItem tri;
    tri.color = { color[0] / 255.0, color[1] / 255.0, color[2] / 255.0, color[3] / 255.0 };
    tri.scale = scale;
    tri.pos = { pos[0], pos[1] };
    tri.fontIdentifier = font_identifier;
    tri.text = text;
    renderData.textItems.push_back(tri);
}
