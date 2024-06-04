#include "Sprite.h"
#include "Shader.h"
#include "SDL_image.h"
#include <iostream>

Sprite::Sprite(SDL_Renderer* renderer) {
	myTexture = NULL;
	x = 20.0f;
	y = 20.0f;
	x_scale = 1.0;
	y_scale = 1.0;
	rotation = 0.0f;
	SDL_Surface* tmpSurface = IMG_Load("images/x.png");
	myTexture = SDL_CreateTextureFromSurface(renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	myRenderer = renderer;
}

void Sprite::render(GLuint VAO) {

	GLuint baseTextureID = convertSDLTextureToGLTexture(myTexture, myRenderer);

	float time = SDL_GetTicks() / 1000.f;

	GLuint shadedTextureID = applyShaderToTexture(SHADER_PROGRAM_RIPPLE, baseTextureID, { time });
	renderTexture(shadedTextureID, VAO, { 30.0f, 30.0f }, { 400.0f, 400.0f }, 0);

	glDeleteTextures(1, &baseTextureID);
	glDeleteTextures(1, &shadedTextureID);

}