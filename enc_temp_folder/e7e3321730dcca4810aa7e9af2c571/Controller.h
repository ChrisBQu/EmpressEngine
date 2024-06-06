#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SDL.h>
#include <map>

const int MAX_NUMBER_OF_CONTROLLER_BUTTONS = 24;

class Controller {
public:
	Controller();
	void findController();
	void handleDeviceConnections(SDL_Event ev);
	void handleInput(SDL_Event ev);
	void bindKeyToButton(SDL_Keycode from, SDL_GameControllerButton to);
	bool getPressed(SDL_GameControllerButton b);
	bool getButton(SDL_GameControllerButton b);
	bool getReleased(SDL_GameControllerButton b);
private:
	std::map<SDL_Keycode, SDL_GameControllerButton> bindings;
	bool buttons_pressed[MAX_NUMBER_OF_CONTROLLER_BUTTONS];
	bool buttons_down[MAX_NUMBER_OF_CONTROLLER_BUTTONS];
	bool buttons_released[MAX_NUMBER_OF_CONTROLLER_BUTTONS];
	SDL_GameController* internalController;
};

#endif // CONTROLLER_H