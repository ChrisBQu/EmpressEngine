#ifndef FONT_H
#define FONT_H

#include <string>
#include <vector>

#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>


extern int recentFontWidth;
extern int recentFontHeight;
extern GLuint fontTextureID;

struct FontTexture {
	glm::vec2 size;
	glm::vec2 pos;
	SDL_Surface* surf;
};

class FontManager {
public:
	FontManager();
	~FontManager();
	int initFont();
	int makeTexture(std::string, glm::vec2 pos, glm::vec3 color);
	void bindFontTextureToGLTexture(FontTexture ft, GLuint& texID);
	void clearFontTextures();
	std::vector<FontTexture> fontTextures;
};

extern FontManager fontManager;

#endif // FONT_H