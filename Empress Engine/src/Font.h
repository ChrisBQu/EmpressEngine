#ifndef FONT_H
#define FONT_H

#include <SDL.h>
#include <GL/glew.h>

extern int recentFontWidth;
extern int recentFontHeight;
extern GLuint fontTextureID;

void initFont();
void drawFont();

#endif