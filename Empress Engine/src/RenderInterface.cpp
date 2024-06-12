#include "Logger.h"
#include "RenderInterface.h"
#include "Sprite.h"

#include <cmath>

RenderData renderData;

std::vector<TileTransform> renderTileData;


glm::mat4 makeOrthographicProjectionMatrix(float left, float right, float top, float bottom) {
    glm::mat4 result(1.0f);
    float near = -1000.0f;
    float far = 1000.0f;
    result[0][0] = 2.0f / (right - left);
    result[1][1] = 2.0f / (top - bottom);
    result[2][2] = 2.0f / (near - far);  // This line changes to handle the near and far planes
    result[3][0] = -(right + left) / (right - left);
    result[3][1] = -(top + bottom) / (top - bottom);  // Fix to correct sign for y-axis
    result[3][2] = (far + near) / (near - far);  // This line changes to handle the near and far planes
    result[3][3] = 1.0f;
    return result;
}

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