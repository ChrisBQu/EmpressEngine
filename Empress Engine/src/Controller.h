#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SDL.h>

#include <map>
#include <vector>

enum class ControllerButton {
	UP = 0,
	DOWN,
	LEFT,
	RIGHT,
	A,
	B,
	X,
	Y,
	L,
	R,
	SELECT,
	START,
	BUTTON_COUNT
};

enum class ControlInputType {
	KEYBOARD_KEY,
	CONTROLLER_BUTTON,
	CONTROL_STICK_TILT
};

struct ControlInputBinding {
	ControlInputType buttonType;
	int port;
	int from;
	ControllerButton to;
};

class Controller {
public:
	Controller();
	void initDefaultControls();
	void bindButton(SDL_Keycode from, ControllerButton to);
	void bindButton(int port, int from, ControllerButton to);
	void handleInput(SDL_Event ev);
	void handleDeviceConnections(SDL_Event ev);
	bool getPressed(ControllerButton b);
	bool getButton(ControllerButton);
	bool getReleased(ControllerButton);
private:
	SDL_JoystickID mainDevice;
	std::map<SDL_JoystickID, SDL_Joystick*> myDevices;
	std::vector<ControlInputBinding> bindings;
	bool buttons_pressed[(int)ControllerButton::BUTTON_COUNT];
	bool buttons_down[(int)ControllerButton::BUTTON_COUNT];
	bool buttons_released[(int)ControllerButton::BUTTON_COUNT];

};

#endif // CONTROLLER_H