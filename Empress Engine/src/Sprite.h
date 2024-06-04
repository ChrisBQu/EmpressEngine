#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>
#include <GL/glew.h>

class Sprite {
public:
	Sprite(SDL_Renderer *renderer);
	void render(GLuint VAO);
private:
	SDL_Renderer* myRenderer;
	float rotation;
	float x;
	float y;
	float width;
	float height;
	float x_scale;
	float y_scale;
	SDL_Texture* myTexture;

};

#endif // SPRITE_H