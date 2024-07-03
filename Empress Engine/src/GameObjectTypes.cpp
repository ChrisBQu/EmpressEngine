#include "GameObjectTypes.h"

#include "Logger.h"

#include "GameObjects/GameObj_TestCharacter.h"
#include "GameObjects/GameObj_TestHUD.h"
#include "GameObjects/GameObj_TestWall.h"

std::map<std::string, GameObjectType> StringToGameObjectTypeMap = {
	{ "TestCharacter" , GameObjectType::TestCharacter },
	{ "Wall", GameObjectType::Wall },
	{ "HUD", GameObjectType::TestHUD }
};

GameObjectType stringToGameObjectType(std::string s) {
	if (StringToGameObjectTypeMap.find(s) == StringToGameObjectTypeMap.end()) {
		LOG_ERROR("GameObjectType does not exist: ", s);
	}
	return StringToGameObjectTypeMap[s];
}

GameObject* generateGameObject(GameObjectType got, float x, float y) {

	GameObject* newobj;

	switch (got) {
	case GameObjectType::TestCharacter:
		newobj = (new GameObject_Character);
		break;
	case GameObjectType::Wall:
		newobj = (new GameObject_Wall);
		break;
	case GameObjectType::TestHUD:
		newobj = (new GameObject_HUD);
		break;
	default:
		LOG_ERROR("Attempted to generate an object with type not accounted for: ", (int)got);
		return nullptr;
	}

	newobj->type = got;
	newobj->x = x;
	newobj->y = y;
	return newobj;

}
