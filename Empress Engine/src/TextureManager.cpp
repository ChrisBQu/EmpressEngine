#include "TextureManager.h"

#include "Logger.h"

#define STB_IMAGE_IMPLEMENTATION
#include "3rdParty/stb_image.h"

TextureManager::TextureManager() {}

int TextureManager::loadTexture(const char* identifier, const char* path) {;
    int width, height, channels;
    char* data = (char*)stbi_load(path, &width, &height, &channels, 4);
    if (!data) { 
        LOG_ERROR("Failed to load texture: ", path);
        return 1;
    }
    LoadedTexture newtexture = { width, height, channels, data };
    if (myTextures.find(identifier) != myTextures.end()) {
        freeTexture(identifier);
    }
    myTextures[identifier] = newtexture;
    return 0;
}

int TextureManager::freeTexture(const char* identifier) {
    if (myTextures.find(identifier) == myTextures.end()) {
        LOG_ERROR("Tried to free the memory of a texture that doesn't exist: ", identifier);
        return 1;
    }
    char* ptr = myTextures[identifier].data;
    myTextures.erase(identifier);
    stbi_image_free(ptr);
    return 0;
}

LoadedTexture TextureManager::getTexture(const char* identifier) {
    if (myTextures.find(identifier) == myTextures.end()) {
        LOG_ERROR("Tried to retrieve a texture that doesn't exist: ", identifier);
        LoadedTexture dummy_texture = { 0, 0, 0 };
        return dummy_texture;
    }
    return myTextures[identifier];
}
