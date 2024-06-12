#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include "ShaderManager.h"
#include "TextureManager.h"

#include <vector>
#include <SDL.h>

struct GL_Context {
    GLuint VAO;
    GLuint FBO;
    GLuint textureID;
    GLuint transformSBOID;
    GLuint orthoProjectionMatrixID;
    GLuint postProcessingTextureBuffer;
    ShaderManager shaderManager;
    TextureManager textureManager;
};

class GL_Renderer {
public:
	GL_Renderer();
	int init();
    void swapTexture(const char* identifier);
	void render();
private:
    GL_Context glcontext;
};

#endif //GL_RENDERER