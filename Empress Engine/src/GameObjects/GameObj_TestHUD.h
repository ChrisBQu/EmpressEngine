#ifndef GAME_OBJ_TEST_HUD_H
#define GAME_OBH_TEST_HUD_H

#include "../GameObject.h"

class GameObject_HUD : public GameObject {
public:
	GameObject_HUD();
	~GameObject_HUD();
	void onUpdate() override;
	void onRender() override;
};

#endif // GAME_OBJ_TEST_HUD_H