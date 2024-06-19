#ifndef GAME_OBJ_TEST_HUD_H
#define GAME_OBH_TEST_HUD_H

#include "GameObject.h"

class GameObject_HUD : public GameObject {
public:

	GameObject_HUD();
	void update() override;
	void render() override;


private:
	int x;
	int y;
};

#endif // GAME_OBJ_TEST_HUD_H