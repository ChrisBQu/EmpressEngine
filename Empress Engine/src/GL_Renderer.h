#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include "FontManager.h"
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
    GLuint transformSBOID;
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
    void resize(int width, int height);
private:
    void drawText(std::string shader_identifier, std::string font_identifier, std::string text, glm::vec2 pos, glm::vec2 scale, glm::vec4 color);
    FontManager fontManager;
    ShaderManager shaderManager;
    TextureManager textureManager;

    GL_Context glcontext;
};

#endif //GL_RENDERER