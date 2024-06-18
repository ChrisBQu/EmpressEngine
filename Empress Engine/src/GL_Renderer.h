#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include "Font.h"
#include "ShaderManager.h"
#include "TextureManager.h"

#include <vector>

#include <glm/glm.hpp>
#include <GL/glew.h>
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
    GLuint textVAO;
    GLuint textVBO;
};

class GL_Renderer {
public:
	GL_Renderer();
	int init();
    void swapTexture(const char* identifier);
	void render();
private:
    void drawText(std::string shader_identifier, std::string font_identifier, std::string text, float x, float y, float scale, glm::vec3 color);
    FontManager fontManager;
    ShaderManager shaderManager;
    TextureManager textureManager;

    GL_Context glcontext;
};

#endif //GL_RENDERER