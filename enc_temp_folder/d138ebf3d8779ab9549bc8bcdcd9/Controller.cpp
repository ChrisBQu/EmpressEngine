#include "Controller.h"
#include "Logger.h"

Controller::Controller() {
    internalController = nullptr;
};


void Controller::findController() {
    internalController = nullptr;
    for (int i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i)) {
            internalController = SDL_GameControllerOpen(i);
            if (internalController) {
                Logger::log(CONSOLE_COLOR_GREEN, "DEVICE", "Controller connected.");
                break;
            }
        }
    }
}

void Controller::handleDeviceConnections(SDL_Event ev) {
    if (ev.type == SDL_CONTROLLERDEVICEADDED) {
        if (!internalController) {
            internalController = SDL_GameControllerOpen(ev.cdevice.which);
            if (internalController) {
                Logger::log(CONSOLE_COLOR_GREEN, "DEVICE", "Controller connected.");
            }
        }
    }
    else if (ev.type == SDL_CONTROLLERDEVICEREMOVED) {
        if (internalController && ev.cdevice.which == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(internalController))) {
            SDL_GameControllerClose(internalController);
            Logger::log(CONSOLE_COLOR_GREEN, "DEVICE", "Controller disconnected.");
            internalController = nullptr;
        }
    }
}

void Controller::handleInput(SDL_Event ev) {

    if (ev.type == SDL_KEYUP) {
        if (!(bindings.find(ev.key.keysym.sym) == bindings.end())) {
            SDL_GameControllerButton btn = bindings[ev.key.keysym.sym];
            buttons_pressed[btn] = false;
            buttons_down[btn] = false;
            buttons_released[btn] = false;
        }
    }

    if (ev.type == SDL_KEYDOWN) {
        if (!(bindings.find(ev.key.keysym.sym) == bindings.end())) {
            SDL_GameControllerButton btn = bindings[ev.key.keysym.sym];
            if (!buttons_down[btn]) { buttons_pressed[btn] = true; }
            buttons_down[btn] = true;
            buttons_released[btn] = false;
        }
    }

    if (ev.type == SDL_CONTROLLERBUTTONUP) {
        buttons_pressed[ev.cbutton.button] = false;
        buttons_down[ev.cbutton.button] = false;
        buttons_released[ev.cbutton.button] = false;
    }

    if (ev.type == SDL_CONTROLLERBUTTONDOWN) {
        if (!buttons_down[ev.cbutton.button]) { buttons_pressed[ev.cbutton.button] = true; }
        buttons_down[ev.cbutton.button] = true;
        buttons_released[ev.cbutton.button] = false;
    }
}

void Controller::bindKeyToButton(SDL_Keycode from, SDL_GameControllerButton to) {
    bindings[from] = to;
}

bool Controller::getPressed(SDL_GameControllerButton b) { return buttons_pressed[b]; }
bool Controller::getButton(SDL_GameControllerButton b) { return buttons_down[b]; }
bool Controller::getReleased(SDL_GameControllerButton b) { return buttons_released[b]; }
/*

Controller::Controller() {
	for (unsigned int i = 0; i < (int)ControllerButton::BUTTON_COUNT; i++) {
		buttons_pressed[i] = false;
		buttons_down[i] = false;
		buttons_released[i] = false;
	}
}

void Controller::bindButton(int port, int from, ControllerButton to) {
	ControlInputBinding newbinding = { ControlInputType::CONTROLLER_BUTTON, port, from, to };
	bindings.push_back(newbinding);
}

void Controller::bindButton(SDL_Keycode from, ControllerButton to) {
	ControlInputBinding newbinding = { ControlInputType::KEYBOARD_KEY, 0, from, to };
	bindings.push_back(newbinding);
}

void Controller::initDefaultControls() {

    // Keyboard
    bindButton(SDLK_UP, ControllerButton::UP);
    bindButton(SDLK_DOWN, ControllerButton::DOWN);
    bindButton(SDLK_LEFT, ControllerButton::LEFT);
    bindButton(SDLK_RIGHT, ControllerButton::RIGHT);
    bindButton(SDLK_a, ControllerButton::A);
    bindButton(SDLK_s, ControllerButton::B);
    bindButton(SDLK_d, ControllerButton::X);
    bindButton(SDLK_f, ControllerButton::Y);

    // Controller
    bindButton(0, 11, ControllerButton::UP);
    bindButton(0, 12, ControllerButton::DOWN);
    bindButton(0, 13, ControllerButton::LEFT);
    bindButton(0, 14, ControllerButton::RIGHT);
    bindButton(0, 0, ControllerButton::A);
    bindButton(0, 1, ControllerButton::B);
    bindButton(0, 2, ControllerButton::X);
    bindButton(0, 3, ControllerButton::Y);
}

void Controller::handleInput(SDL_Event ev) {

    if (ev.type == SDL_KEYDOWN) {
        for (ControlInputBinding each_binding : bindings) {
            if (each_binding.buttonType == ControlInputType::KEYBOARD_KEY && ev.key.keysym.sym == each_binding.from) {
                if (!buttons_down[(int)each_binding.to]) {
                    buttons_pressed[(int)each_binding.to] = true;
                }
                buttons_down[(int)each_binding.to] = true;
            }
        }
    }

    if (ev.type == SDL_KEYUP) {
        for (ControlInputBinding each_binding : bindings) {
            if (each_binding.buttonType == ControlInputType::KEYBOARD_KEY && ev.key.keysym.sym == each_binding.from) {
                buttons_released[(int)each_binding.to] = true;
                buttons_down[(int)each_binding.to] = false;
            }
        }
    }

    if (ev.type == SDL_JOYBUTTONDOWN) {
        for (ControlInputBinding each_binding : bindings) {
            if (each_binding.buttonType == ControlInputType::CONTROLLER_BUTTON && (int)ev.jbutton.button == each_binding.from) {
                if ((ev.jbutton.which == each_binding.port) || (each_binding.port == 0 && ev.jbutton.which == mainDevice)) {
                    buttons_released[(int)each_binding.to] = true;
                    buttons_down[(int)each_binding.to] = false;
                }
            }
        }
    }

    if (ev.type == SDL_JOYBUTTONUP) {
        for (ControlInputBinding each_binding : bindings) {
            if (each_binding.buttonType == ControlInputType::CONTROLLER_BUTTON && (int)ev.jbutton.button == each_binding.from) {
                if ((ev.jbutton.which == each_binding.port) || (each_binding.port == 0 && ev.jbutton.which == mainDevice)) {
                    buttons_released[(int)each_binding.to] = true;
                    buttons_down[(int)each_binding.to] = false;
                }
            }
        }
    }


}

bool Controller::getPressed(ControllerButton b) {
	return buttons_pressed[(int)b];
}

bool Controller::getButton(ControllerButton b) { 
	return buttons_down[(int)b]; 
}

bool Controller::getReleased(ControllerButton b) {
	return buttons_released[(int)b];
}

void Controller::handleDeviceConnections(SDL_Event ev) {
    SDL_Joystick* joystick;
    SDL_JoystickID joystick_id;
    if (ev.type == SDL_JOYDEVICEADDED) {

        joystick = SDL_JoystickOpen(ev.jdevice.which);
        if (joystick) {
            joystick_id = SDL_JoystickInstanceID(joystick);
            myDevices[joystick_id] = joystick;
            Logger::log(CONSOLE_COLOR_GREEN, "DEVICE", "New controller detected with ID (", joystick_id, ")");

            if (myDevices.size() == 1) {
                mainDevice = joystick_id;
                Logger::log(CONSOLE_COLOR_GREEN, "DEVICE", "Primary controller has been connected.");
            }

        }

    }

    if (ev.type == SDL_JOYDEVICEREMOVED) {

        Logger::log(CONSOLE_COLOR_GREEN, "DEVICE", "Controller removed with ID (", ev.jdevice.which, ")");
        if (myDevices.find(ev.jdevice.which) == myDevices.end()) {
            LOG_WARNING("Controller removed had never been detected, and added? Device pointers may be corrupted.");
        }
        else {
            SDL_JoystickClose(myDevices[ev.jdevice.which]);
            myDevices.erase(ev.jdevice.which);
        }

        if (myDevices.size() == 0) { 
            mainDevice = -1; 
            Logger::log(CONSOLE_COLOR_GREEN, "DEVICE", "Primary controller has been disconnected.");
        }

    }
}

*/

