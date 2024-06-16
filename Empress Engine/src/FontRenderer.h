#ifndef FONT_RENDERER_H
#define FONT_RENDERER_H

#include <string>
#include <vector>

#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SDL_ttf.h>


extern int recentFontWidth;
extern int recentFontHeight;
extern GLuint fontTextureID;

struct FontTexture {
	glm::vec2 size;
	glm::vec2 pos;
	SDL_Surface* surf;
};

class FontRenderer {
public:
	FontRenderer();
	~FontRenderer();
	int initFont();
	int makeTexture(std::string, glm::vec2 pos, glm::vec3 color);
	void bindFontTextureToGLTexture(FontTexture ft, GLuint& texID);
	void clearFontTextures();
	std::vector<FontTexture> fontTextures;
private:
	TTF_Font* theFont;
};

#endif // FONT_RENDERER_H