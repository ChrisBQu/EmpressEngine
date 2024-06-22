#include "Tileset.h"
#include "RenderInterface.h"
#include "Scene.h"

#include <algorithm>

// Default Constructor
TileLayer::TileLayer() {
    myTSD = { "_null_", {16, 16}, 1, {} };
    myTLD = { 0, 1, {0, 0}, { } };
    active = false;
}

TileLayer::TileLayer(TilesetData tsd, TilesetLayerData tld) {
    myTSD = tsd;
    myTLD = tld;
    active = false;
}

void TileLayer::render(int framecount) { 
    // Camera bounds calculation
    OrthographicCamera cam = getLoadedScene()->getCamera();
    float cameraLeft = cam.pos.x - cam.dimensions.x / 2.0f;
    float cameraRight = cam.pos.x + cam.dimensions.x / 2.0f;
    float cameraBottom = cam.pos.y - cam.dimensions.y / 2.0f;
    float cameraTop = cam.pos.y + cam.dimensions.y / 2.0f;

    // Calculate the number of tiles that fit in the camera view plus an extra tile on each side for partially visible tiles
    int firstCol = std::max(0, (int)std::floor((cameraLeft - myTLD.pos.x) / myTSD.atlasSize.x));
    int lastCol = std::min((int)myTLD.tilesPerRow - 1, (int)std::ceil((cameraRight - myTLD.pos.x) / myTSD.atlasSize.x));
    int firstRow = std::max(0, (int)std::floor((cameraBottom - myTLD.pos.y) / myTSD.atlasSize.y));
    int lastRow = std::min((int)(myTLD.tileData.size() / myTLD.tilesPerRow) - 1, (int)std::ceil((cameraTop - myTLD.pos.y) / myTSD.atlasSize.y));

    // Tile drawing loop
    for (int row = firstRow; row <= lastRow; row++) {
        for (int col = firstCol; col <= lastCol; col++) {
            unsigned int i = row * myTLD.tilesPerRow + col;
            if (i >= myTLD.tileData.size()) { continue; }

            if (framecount % myTSD.animatedTileMap[myTLD.tileData[i]].second == 0) {
                myTLD.tileData[i] = myTSD.animatedTileMap[myTLD.tileData[i]].first;
            }
            unsigned int tex_col = myTLD.tileData[i] % myTSD.framesPerRow;
            unsigned int tex_row = myTLD.tileData[i] / myTSD.framesPerRow;

            // Calculate the position
            float pos_x = col * myTSD.atlasSize.x + myTLD.pos.x;
            float pos_y = row * myTSD.atlasSize.y + myTLD.pos.y;

            // Correct bounding box calculation
            float tileLeft = pos_x - myTSD.atlasSize.x / 2.0f;
            float tileRight = pos_x + myTSD.atlasSize.x / 2.0f;
            float tileBottom = pos_y - myTSD.atlasSize.y / 2.0f;
            float tileTop = pos_y + myTSD.atlasSize.y / 2.0f;

            // Check if the tile is within the camera view
            if (tileRight > cameraLeft && tileLeft < cameraRight + myTSD.atlasSize.x && tileTop > cameraBottom && tileBottom < cameraTop + myTSD.atlasSize.y) {
                // Draw the tile
                drawQuad(myTSD.atlasIdentifier, { myTSD.atlasSize.x * tex_col, myTSD.atlasSize.y * tex_row }, myTSD.atlasSize, { pos_x, pos_y }, { myTSD.atlasSize.x, myTSD.atlasSize.y }, myTLD.depth, 0.0f, 1.0f);
            }
        }
    }
}

