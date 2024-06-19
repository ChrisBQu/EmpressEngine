#include "FontManager.h"
#include "Logger.h"
#include "RenderInterface.h"
#include "Sprite.h"
#include "Utility.h"

#include "GameConfig.h"

#include <cmath>

RenderData renderData;

void drawQuad(std::string tex_identifier, glm::vec2 tex_pos, glm::vec2 tex_size, glm::vec2 pos, glm::vec2 scale, int depth, float rotation, float alpha) {
    RenderTransform transform;
    transform.pos = pos - glm::vec2(tex_size) / 2.0f;
    transform.size = glm::vec2(tex_size[0] * scale.x, tex_size[1] * scale.y);
    transform.atlasOffset = tex_pos;
    transform.spriteSize = tex_size;
    transform.depth = (depth + 100.0) / 200.0;
    transform.rotation = rotation;
    transform.alpha = alpha;
    renderData.transforms[tex_identifier].push_back(transform);
}

void drawUIQuad(std::string tex_identifier, glm::vec2 tex_pos, glm::vec2 tex_size, glm::vec2 pos, glm::vec2 scale, int depth, float rotation, float alpha) {

    float xs = (float)gameConfig.screenWidth / (float)DEFAULT_SCREEN_WIDTH;
    float ys = (float)gameConfig.screenHeight / (float)DEFAULT_SCREEN_HEIGHT;

    RenderTransform transform;
    // Adjust the position so that the top-left corner is at the specified pos
    transform.pos = pos;
    transform.pos = { transform.pos[0] * xs, transform.pos[1] * ys };
    transform.size = glm::vec2(tex_size[0] * scale.x * xs, tex_size[1] * scale.y * ys);
    transform.atlasOffset = tex_pos;
    transform.spriteSize = tex_size;
    transform.depth = (depth + 100.0) / 200.0;
    transform.rotation = rotation;
    transform.alpha = alpha;
    renderData.uitransforms[tex_identifier].push_back(transform);
}





void drawTileset(TilesetData tsd, std::vector<unsigned int>& tile_indices, glm::vec2 offset_pos, unsigned int tiles_per_row, unsigned int framecount, unsigned int depth) {
    // Camera bounds calculation
    float cameraLeft = renderData.gameCamera.pos.x - renderData.gameCamera.dimensions.x / 2.0f;
    float cameraRight = renderData.gameCamera.pos.x + renderData.gameCamera.dimensions.x / 2.0f;
    float cameraBottom = renderData.gameCamera.pos.y - renderData.gameCamera.dimensions.y / 2.0f;
    float cameraTop = renderData.gameCamera.pos.y + renderData.gameCamera.dimensions.y / 2.0f;

    // Calculate the number of tiles that fit in the camera view plus an extra tile on each side for partially visible tiles
    int firstCol = std::max(0, (int)std::floor((cameraLeft - offset_pos.x) / tsd.atlasSize.x));
    int lastCol = std::min((int)tiles_per_row - 1, (int)std::ceil((cameraRight - offset_pos.x) / tsd.atlasSize.x));
    int firstRow = std::max(0, (int)std::floor((cameraBottom - offset_pos.y) / tsd.atlasSize.y));
    int lastRow = std::min((int)(tile_indices.size() / tiles_per_row) - 1, (int)std::ceil((cameraTop - offset_pos.y) / tsd.atlasSize.y));

    // Tile drawing loop
    for (int row = firstRow; row <= lastRow; row++) {
        for (int col = firstCol; col <= lastCol; col++) {
            unsigned int i = row * tiles_per_row + col;
            if (i >= tile_indices.size()) { continue; }

            if (framecount % tsd.animatedTileMap[tile_indices[i]].second == 0) {
                tile_indices[i] = tsd.animatedTileMap[tile_indices[i]].first;
            }
            unsigned int tex_col = tile_indices[i] % tsd.framesPerRow;
            unsigned int tex_row = tile_indices[i] / tsd.framesPerRow;

            // Calculate the position
            float pos_x = col * tsd.atlasSize.x + offset_pos.x;
            float pos_y = row * tsd.atlasSize.y + offset_pos.y;

            // Correct bounding box calculation
            float tileLeft = pos_x - tsd.atlasSize.x / 2.0f;
            float tileRight = pos_x + tsd.atlasSize.x / 2.0f;
            float tileBottom = pos_y - tsd.atlasSize.y / 2.0f;
            float tileTop = pos_y + tsd.atlasSize.y / 2.0f;

            // Check if the tile is within the camera view
            if (tileRight > cameraLeft && tileLeft < cameraRight + tsd.atlasSize.x && tileTop > cameraBottom && tileBottom < cameraTop + tsd.atlasSize.y) {
                // Draw the tile
                drawQuad(tsd.atlasIdentifier, { tsd.atlasSize.x * tex_col, tsd.atlasSize.y * tex_row }, tsd.atlasSize, { pos_x, pos_y }, { 1.0f, 1.0f }, depth, 0.0f, 1.0f);
            }
        }
    }
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
