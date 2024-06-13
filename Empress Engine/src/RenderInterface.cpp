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
    transform.depth = (depth + 100.0) / 200.0;
    transform.rotation = rotation;
    transform.alpha = alpha;
    renderData.transforms[tex_identifier].push_back(transform);
}


void drawTileset(TilesetData tsd, std::vector<unsigned int> &tile_indices, unsigned int tiles_per_row, unsigned int framecount, unsigned int depth) {
    for (unsigned int i = 0; i < tile_indices.size(); i++) {
        if (framecount % tsd.animatedTileMap[tile_indices[i]].second == 0) {
            tile_indices[i] = tsd.animatedTileMap[tile_indices[i]].first;
        }
        unsigned int tex_col = tile_indices[i] % tsd.framesPerRow;
        unsigned int tex_row = tile_indices[i] / tsd.framesPerRow;
        unsigned int screen_col = (i % tiles_per_row)+1;
        unsigned int screen_row = (i / tiles_per_row)+1;

        unsigned int pos_x = screen_col * tsd.atlasSize.x;
        unsigned int pos_y = screen_row * tsd.atlasSize.y;

        float cameraLeft = renderData.gameCamera.pos.x - renderData.gameCamera.dimensions.x / 2.0f;
        float cameraRight = renderData.gameCamera.pos.x + renderData.gameCamera.dimensions.x / 2.0f;
        float cameraBottom = renderData.gameCamera.pos.y - renderData.gameCamera.dimensions.y / 2.0f;
        float cameraTop = renderData.gameCamera.pos.y + renderData.gameCamera.dimensions.y / 2.0f;

        float tileLeft = pos_x - tsd.atlasSize.x;
        float tileRight = pos_x + tsd.atlasSize.x;
        float tileBottom = pos_y - tsd.atlasSize.y;
        float tileTop = pos_y + tsd.atlasSize.y;

        if (tileRight > cameraLeft && tileLeft < cameraRight && tileTop > cameraBottom && tileBottom < cameraTop) {

            drawQuad(tsd.atlasIdentifier, { tsd.atlasSize.x * tex_col, tsd.atlasSize.y * tex_row }, tsd.atlasSize, { pos_x, pos_y }, { 1.0f, 1.0f }, depth, 0.0f, 1.0f);

        }
    }

}