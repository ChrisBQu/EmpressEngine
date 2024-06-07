#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SDL.h>
#include <map>
#include <string>

const int CONTROL_STICK_PRESS_THRESHOLD = 8000;

struct ControlStickMovement {
	int stick;
	int axis;
	int direction;

	// Make the struct mappable
	bool operator<(const ControlStickMovement& other) const {
		if (stick != other.stick) return stick < other.stick;
		if (axis != other.axis) return axis < other.axis;
		return direction < other.direction;
	}
};

const int MAX_NUMBER_OF_CONTROLLER_BUTTONS = 24;

class Controller {
public:
	Controller();
	void findController();
	void handleDeviceConnections(SDL_Event ev);
	void handleInput(SDL_Event ev);
	void bindKeyToButton(SDL_Keycode from, SDL_GameControllerButton to);
	void bindStickToButton(int from_stick, int from_axis, int from_direction, SDL_GameControllerButton to);
	void initDefaultKeyBindings();
	bool getPressed(SDL_GameControllerButton b);
	bool getButton(SDL_GameControllerButton b);
	bool getReleased(SDL_GameControllerButton b);
private:
	std::map<SDL_Keycode, SDL_GameControllerButton> bindings;
	std::map<struct ControlStickMovement, SDL_GameControllerButton> stickBindings;
	bool buttons_pressed[MAX_NUMBER_OF_CONTROLLER_BUTTONS];
	bool buttons_down[MAX_NUMBER_OF_CONTROLLER_BUTTONS];
	bool buttons_released[MAX_NUMBER_OF_CONTROLLER_BUTTONS];
	SDL_GameController* internalController;
};

#endif // CONTROLLER_H