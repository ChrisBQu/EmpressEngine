#ifndef TILESET_H
#define TILESET_H

#include <map>
#include <string>
#include <utility>
#include <glm/glm.hpp>


struct TilesetData {
	std::string atlasIdentifier;
	glm::ivec2 atlasSize;
	unsigned int framesPerRow;
	unsigned int ticksPerFrame;
	std::map<unsigned int, std::pair<unsigned int, unsigned int>> animatedTileMap;

};


#endif // TILESET_H
