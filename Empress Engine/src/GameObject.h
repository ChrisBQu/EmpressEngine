#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "Sprite.h"
#include "TransformStruct.h"

#include <vector>

class GameObject {
public:
	GameObject();
	void update();
	void render();

	// Variables
	bool visible;
   
	// Components
	TransformStruct transform;
	Sprite sprite;
};

#endif // GAME_OBJECT_H