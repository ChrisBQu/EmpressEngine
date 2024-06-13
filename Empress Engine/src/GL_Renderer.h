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
    GLuint fontAtlasID;
    GLuint transformSBOID;
    GLuint orthoProjectionMatrixID;
    GLuint postProcessingTextureBuffer;
    GLuint postProcessingDepthBuffer;
};

class GL_Renderer {
public:
	GL_Renderer();
	int init();
    void swapTexture(const char* identifier);
	void render();
private:
    ShaderManager shaderManager;
    TextureManager textureManager;
    GL_Context glcontext;
};

#endif //GL_RENDERER