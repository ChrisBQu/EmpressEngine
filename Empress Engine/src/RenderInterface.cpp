#include "Logger.h"
#include "RenderInterface.h"
#include "Sprite.h"

#include <cmath>

RenderData renderData;

void drawQuad(std::string tex_identifier, glm::vec2 tex_pos, glm::vec2 tex_size, glm::vec2 pos, glm::vec2 scale, int depth, float rotation, float alpha) {
    RenderTransform transform;
    transform.pos = pos - glm::vec2(tex_size) / 2.0f;
    transform.size = glm::vec2(tex_size[0] * scale.x, tex_size[1] * scale.y);
    transform.atlasOffset = tex_pos;
    transform.spriteSize = tex_size;
    transform.depth = -1.0 / (1.0 + std::exp(-depth)); // Map an integer to a float in range [0, 1]
    transform.rotation = rotation;
    transform.alpha = alpha;
    renderData.transforms[tex_identifier].push_back(transform);
}