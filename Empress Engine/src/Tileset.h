#ifndef TILESET_H
#define TILESET_H

#include <map>
#include <string>
#include <utility>
#include <glm/glm.hpp>
#include <vector>

// Struct contains metadata relating to the tileset's texture
struct TilesetData {
	std::string atlasIdentifier;
	glm::ivec2 atlasSize;
	unsigned int framesPerRow;
	std::map<unsigned int, std::pair<unsigned int, unsigned int>> animatedTileMap;
};

// Struct contains data for the layout of tiles on the screen
struct TilesetLayerData {
	int depth;
	unsigned int tilesPerRow;
	glm::vec2 pos;
	std::vector<unsigned int> tileData;
};

class TileLayer {
public:
	TileLayer();
	TileLayer(TilesetData tsd, TilesetLayerData tld);
	void render(int framecount);

	TilesetData myTSD;
	TilesetLayerData myTLD;
};

TilesetData getTilesetData(std::string identifier);
void loadTilesetManifest(const char* filename);

#endif // TILESET_H
