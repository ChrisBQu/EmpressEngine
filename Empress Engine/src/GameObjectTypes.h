#ifndef GAME_OBJECT_TYPES_H
#define GAME_OBJECT_TYPES_H

#include <map>
#include <string>

// Forward declaration
class GameObject;

enum class GameObjectType {
	_null_,
	TestCharacter,
	TestHUD,
	Wall
};


GameObjectType stringToGameObjectType(std::string s);
GameObject* generateGameObject(GameObjectType got, float x, float y);

#endif // GAME_OBJECT_TYPES