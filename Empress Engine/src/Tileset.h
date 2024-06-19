#ifndef TILESET_H
#define TILESET_H

#include <map>
#include <string>
#include <utility>
#include <glm/glm.hpp>
#include <vector>


struct TilesetData {
	std::string atlasIdentifier;
	glm::ivec2 atlasSize;
	unsigned int framesPerRow;
	std::map<unsigned int, std::pair<unsigned int, unsigned int>> animatedTileMap;
};

struct TilesetLayerData {
	int depth;
	unsigned int tilesPerRow;
	glm::vec2 pos;
	std::vector<unsigned int> tileData;
	TilesetData tilesetData;
};

#endif // TILESET_H
