#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include <vector>

#include <SDL.h>
#include <GL/glew.h>


void initGLRenderer();
void createShaders();
void glRender();

extern GLuint SHADER_PROGRAM_DEFAULT;
extern GLuint SHADER_PROGRAM_RIPPLE;


#endif //GL_RENDERER