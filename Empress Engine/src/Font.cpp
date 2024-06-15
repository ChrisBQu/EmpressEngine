#include "Font.h"
#include "Logger.h"

#include <SDL_ttf.h>
#include <GL/glew.h>

TTF_Font* theFont;

FontManager fontManager;

// Constructor
FontManager::FontManager() {};

int FontManager::initFont() {
    if (TTF_Init() == -1) {
        LOG_ERROR("Failed to initialize TTF: ", TTF_GetError());
        return 1;
    }

    theFont = TTF_OpenFont("assets/fonts/public_pixel.ttf", 28);
    if (!theFont) {
        LOG_ERROR("Failed to open font: ", TTF_GetError());
        return 1;
    }

    return 0;
}

// Draw the text using the font
int FontManager::makeTexture(std::string text, glm::vec2 pos, glm::vec3 color) {

    FontTexture newFontTexture;
    newFontTexture.pos = pos;

    SDL_Surface* theSurface;

    if (!theFont) {
        LOG_ERROR("Font not initialized.");
        return 1;
    }

    SDL_Color textColor = { color[0], color[1], color[2], 255 };
    theSurface = TTF_RenderText_Blended(theFont, text.c_str(), textColor);
    if (!theSurface) {
        LOG_ERROR("Failed to create surface: ", TTF_GetError());
        return 1;
    }

    // Convert surface to a format suitable for OpenGL
    SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(theSurface, SDL_PIXELFORMAT_ABGR8888, 0);
    if (!formattedSurface) {
        LOG_ERROR("Failed to convert surface format: ", SDL_GetError());
        SDL_FreeSurface(theSurface);
        return 1;
    }

    newFontTexture.size = { formattedSurface->w, formattedSurface->h };
    newFontTexture.surf = formattedSurface;
    // Free the surfaces after use
    //SDL_FreeSurface(formattedSurface);
    SDL_FreeSurface(theSurface);

    fontTextures.push_back(newFontTexture);

    return 0;
}

void FontManager::bindFontTextureToGLTexture(FontTexture ft, GLuint &texID) {
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set the texture parameters (wrapping, filtering)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ft.surf->w, ft.surf->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, ft.surf->pixels);
}

void FontManager::clearFontTextures() {
    for (FontTexture& each_texture : fontTextures) {
        SDL_FreeSurface(each_texture.surf);
    }
    fontTextures.clear();
}

// Destructor
FontManager::~FontManager() {
    if (theFont) {
        TTF_CloseFont(theFont);
        theFont = nullptr;
    }
    TTF_Quit();
}

