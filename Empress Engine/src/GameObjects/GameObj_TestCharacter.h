#ifndef GAME_OBJ_TEST_CHARACTER_H
#define GAME_OBH_TEST_CHARACTER_H

#include "../GameObject.h"

class GameObject_Character : public GameObject {
public:
	GameObject_Character();
	~GameObject_Character();

	void trigger_onPressedLeft();
	void trigger_onPressedRight();
	void trigger_onHeldRight();
	void trigger_onHeldLeft();
	void trigger_onReleasedLeft();
	void trigger_onReleasedRight();


};

#endif // GAME_OBJ_TEST_CHARACTER_H