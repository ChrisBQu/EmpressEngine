#include "FontManager.h"
#include "GameConfig.h"
#include "Logger.h"
#include "RenderInterface.h"
#include "Scene.h"
#include "Utility.h"
#include <cmath>

RenderData renderData;

void drawQuad(std::string tex_identifier, glm::vec2 tex_pos, glm::vec2 tex_size, glm::vec2 pos, glm::vec2 size, int depth, float rotation, float alpha) {
    RenderTransform transform;
    transform.pos =  pos - glm::vec2(tex_size) / 2.0f;
    transform.size = size;
    transform.atlasOffset = tex_pos;
    transform.spriteSize = tex_size;
    transform.rotation = rotation;
    transform.alpha = alpha;
    renderData.transforms[RENDERING_DEPTH_LAYERS-(depth+(RENDERING_DEPTH_LAYERS/2))][tex_identifier].push_back(transform);
}

void drawUIQuad(std::string tex_identifier, glm::vec2 tex_pos, glm::vec2 tex_size, glm::vec2 pos, glm::vec2 size, int depth, float rotation, float alpha) {
    float xs = (float)gameConfig.screenWidth / (float)DEFAULT_SCREEN_WIDTH;
    float ys = (float)gameConfig.screenHeight / (float)DEFAULT_SCREEN_HEIGHT;
    RenderTransform transform;
    transform.pos = pos;
    transform.pos = { transform.pos[0] * xs, transform.pos[1] * ys };
    transform.size = size;
    transform.atlasOffset = tex_pos;
    transform.spriteSize = tex_size;
    transform.rotation = rotation;
    transform.alpha = alpha;
    renderData.uitransforms[RENDERING_DEPTH_LAYERS - (depth + (RENDERING_DEPTH_LAYERS / 2))][tex_identifier].push_back(transform);
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

void drawLine(glm::vec2 start, glm::vec2 end, float thickness, glm::vec4 color) {
    glm::vec2 direction = end - start;
    glm::vec2 normal = glm::vec2(-direction.y, direction.x);
    normal = glm::normalize(normal);
    float halfThickness = thickness / 2.0f;
    PrimitiveQuadRenderItem pqri;
    pqri.point1 = start + normal * halfThickness;   // Top-left
    pqri.point2 = end + normal * halfThickness;     // Top-right
    pqri.point3 = end - normal * halfThickness;     // Bottom-right
    pqri.point4 = start - normal * halfThickness;   // Bottom-left
    pqri.color = color;
    renderData.primitiveQuadItems.push_back(pqri);
}
