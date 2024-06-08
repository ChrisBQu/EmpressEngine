#include "TextureManager.h"
#include "Logger.h"
#include "Utility.h"

#include <chrono>
#include <thread>

#define STB_IMAGE_IMPLEMENTATION
#include "3rdParty/stb_image.h"

// Constructor
TextureManager::TextureManager() {}

// Destructor
TextureManager::~TextureManager() {
    for (auto& each : myTextures) {
        stbi_image_free(each.second.data);
    }
}

// Take an identifier string, and load the texture from a filepath
int TextureManager::loadTexture(std::string identifier, std::string path) {;
    int width, height, channels;
    char* data = (char*)stbi_load(path.c_str(), &width, &height, &channels, 4);
    if (!data) { 
        LOG_ERROR("Failed to load texture: ", path);
        return 1;
    }
    uint64_t timestamp = getFileTimestamp(path.c_str());
    LoadedTexture newtexture = { width, height, channels, data, timestamp, path };
    if (myTextures.find(identifier) != myTextures.end()) {
        freeTexture(identifier);
    }
    myTextures[identifier] = newtexture;
    return 0;
}

// Free the memory of a texture of a given identifier, and remove it from the loaded texture list
int TextureManager::freeTexture(std::string identifier) {
    if (myTextures.find(identifier) == myTextures.end()) {
        LOG_ERROR("Tried to free the memory of a texture that doesn't exist: ", identifier);
        return 1;
    }
    char* ptr = myTextures[identifier].data;
    myTextures.erase(identifier);
    stbi_image_free(ptr);
    return 0;
}

// Get texture by identifier (GLuint used by OpenGL)
LoadedTexture TextureManager::getTexture(std::string identifier) {
    if (myTextures.find(identifier) == myTextures.end()) {
        LOG_ERROR("Tried to retrieve a texture that doesn't exist: ", identifier);
        LoadedTexture dummy_texture = { 0, 0, 0 };
        return dummy_texture;
    }
    return myTextures[identifier];
}


// Call once per frame so that changes to the texture files will automatically be reflected without having to restart the game
void TextureManager::hotReload() {
    for (auto& each : myTextures) {
        uint64_t newstamp = getFileTimestamp(each.second.path.c_str());
        if (newstamp > each.second.timestamp) {
            Logger::log(CONSOLE_COLOR_GREEN, "TEXTURE", "Hot reload triggered for texture: ", each.first);
            each.second.timestamp = newstamp;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            int width, height, channels;
            char* data = (char*)stbi_load(each.second.path.c_str(), &width, &height, &channels, 4);
            if (stbi_failure_reason()) {
                LOG_ERROR("Failed to hot reload texture: ", each.first);
            }
            else {
                stbi_image_free(each.second.data);
                each.second.data = data;
                each.second.width = width;
                each.second.height = height;
                each.second.channels = channels;
            }     
        }
    }
}