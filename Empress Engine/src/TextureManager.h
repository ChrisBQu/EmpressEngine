#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <map>
#include <string>
#include <sys/stat.h>

struct LoadedTexture {
	int width;
	int height;
	int channels;
	char* data;

	// Texture metadata is used for hot reloading
	uint64_t timestamp;
	std::string path;
};

class TextureManager {
public:
	TextureManager();
	~TextureManager();
	int loadTexture(std::string identifier, std::string path);
	int unloadTexture(std::string identifier);
	void unloadAllTextures();
	LoadedTexture getTexture(std::string identifier);
	void hotReload();
private:
	std::map<std::string, LoadedTexture> myTextures;
};

#endif // TEXTURE_MANAGER_H