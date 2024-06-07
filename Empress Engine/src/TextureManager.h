#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <map>

struct LoadedTexture {
	int width;
	int height;
	int channels;
	char* data;
};

class TextureManager {
public:
	TextureManager();
	int loadTexture(const char* identifier, const char* path);
	int freeTexture(const char* identifier);
	LoadedTexture getTexture(const char* identifier);
private:
	std::map<const char*, LoadedTexture> myTextures;
};

#endif // TEXTURE_MANAGER_H