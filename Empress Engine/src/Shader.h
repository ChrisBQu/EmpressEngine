#ifndef SHADER_H
#define SHADER_H

#include <vector>

#include <SDL.h>
#include <GL/glew.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint generateQuadVAO();
void setupOrthoProjection(unsigned int screenWidth, unsigned int screenHeight);
void createShaders();

GLuint convertSDLTextureToGLTexture(SDL_Texture* imgTexture, SDL_Renderer* renderer);
void renderTexture(GLuint textureID, GLuint quadVAO, const glm::vec2& position, const glm::vec2& scale, float rotation);
GLuint applyShaderToTexture(GLuint shaderProgram, GLuint textureID, const std::vector<float> parameters);

extern GLuint SHADER_PROGRAM_DEFAULT;
extern GLuint SHADER_PROGRAM_RIPPLE;


#endif //SHADER_H