#include "Font.h"
#include "Logger.h"

#include <SDL_ttf.h>
#include <GL/glew.h>

// Declare the font and texture variables
TTF_Font* theFont = nullptr;
SDL_Surface* theSurface = nullptr;
GLuint fontTextureID = 0;

int recentFontWidth = 0;
int recentFontHeight = 0;

// Initialize the font
void initFont() {
    if (TTF_Init() == -1) {
        LOG_ERROR("Failed to initialize TTF: %s", TTF_GetError());
        return;
    }

    theFont = TTF_OpenFont("assets/fonts/public_pixel.ttf", 28);
    if (!theFont) {
        LOG_ERROR("Failed to open font: %s", TTF_GetError());
        return;
    }
}

// Draw the text using the font
void drawFont() {
    if (!theFont) {
        LOG_ERROR("Font not initialized.");
        return;
    }

    SDL_Color textColor = { 255, 0, 0, 255 };  // Magenta
    theSurface = TTF_RenderText_Blended(theFont, "Hello World", textColor); // Use Blended rendering for better quality
    if (!theSurface) {
        LOG_ERROR("Failed to create surface: %s", TTF_GetError());
        return;
    }

    // Convert surface to a format suitable for OpenGL
    SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(theSurface, SDL_PIXELFORMAT_ABGR8888, 0);
    if (!formattedSurface) {
        LOG_ERROR("Failed to convert surface format: %s", SDL_GetError());
        SDL_FreeSurface(theSurface);
        return;
    }

    // Generate and bind texture
    glGenTextures(1, &fontTextureID);
    glBindTexture(GL_TEXTURE_2D, fontTextureID);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set the texture parameters (wrapping, filtering)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Upload the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, formattedSurface->w, formattedSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, formattedSurface->pixels);

    recentFontWidth = formattedSurface->w;
    recentFontHeight = formattedSurface->h;

    // Free the surfaces after use
    SDL_FreeSurface(formattedSurface);
    SDL_FreeSurface(theSurface);
    theSurface = nullptr;  // Avoid dangling pointer
}

// Clean up resources
void cleanupFont() {
    if (theFont) {
        TTF_CloseFont(theFont);
        theFont = nullptr;
    }
    TTF_Quit();

    if (fontTextureID) {
        glDeleteTextures(1, &fontTextureID);
        fontTextureID = 0;
    }
}

