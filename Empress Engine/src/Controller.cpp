#include "Controller.h"
#include "Logger.h"

#include <cmath>

// Constructor
Controller::Controller() {
    internalController = nullptr;
};

// Destructor
Controller::~Controller() {
    if (internalController) {
        SDL_GameControllerClose(internalController);
    }
}

// Default controls map certain keyboard keys, and control stick presses, to controller buttons
void Controller::initDefaultKeyBindings() {
    bindKeyToButton(SDLK_a, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_Y);
    bindKeyToButton(SDLK_s, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_X);
    bindKeyToButton(SDLK_d, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_B);
    bindKeyToButton(SDLK_f, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_A);
    bindKeyToButton(SDLK_z, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
    bindKeyToButton(SDLK_x, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
    bindKeyToButton(SDLK_RETURN, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_START);
    bindKeyToButton(SDLK_LSHIFT, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_BACK);
    bindKeyToButton(SDLK_UP, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP);
    bindKeyToButton(SDLK_DOWN, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN);
    bindKeyToButton(SDLK_LEFT, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT);
    bindKeyToButton(SDLK_RIGHT, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
    bindStickToButton(0, 1, -1, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_UP);
    bindStickToButton(0, 1, 1, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_DOWN);
    bindStickToButton(0, 0, -1, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_LEFT);
    bindStickToButton(0, 0, 1, SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
}

// Call once to attempt to connect to the first available controller
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

// Call once per frame so that controller connects and disconnects are caught and handled
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

// Should be called once per frame before handleInput to clear the pressed/released states from the previous frame
void Controller::resetInput() {
    for (unsigned int i = 0; i < MAX_NUMBER_OF_CONTROLLER_BUTTONS; i++) {
        buttons_pressed[i] = false;
        buttons_released[i] = false;
    }
}

// Call once per frame to handle incoming events and update button press arrays accordingly
void Controller::handleInput(SDL_Event ev) {

    // Keyboard key pressed
    if (ev.type == SDL_KEYDOWN) {
        SDL_GameControllerButton btn = bindings[ev.key.keysym.sym];
        if (!buttons_down[btn]) { buttons_pressed[btn] = true; }
        buttons_down[btn] = true;
        buttons_released[btn] = false; 
    }

    // Keyboard key released
    if (ev.type == SDL_KEYUP) {
        SDL_GameControllerButton btn = bindings[ev.key.keysym.sym];
        buttons_pressed[btn] = false;
        buttons_down[btn] = false;
        buttons_released[btn] = true;
    }

    // Controller button released
    if (ev.type == SDL_CONTROLLERBUTTONUP) {
        buttons_pressed[ev.cbutton.button] = false;
        buttons_down[ev.cbutton.button] = false;
        buttons_released[ev.cbutton.button] = true;
    }

    // Controller button pressed
    if (ev.type == SDL_CONTROLLERBUTTONDOWN) {
        if (!buttons_down[ev.cbutton.button]) { buttons_pressed[ev.cbutton.button] = true; }
        buttons_down[ev.cbutton.button] = true;
        buttons_released[ev.cbutton.button] = false;
    }

    // Control Stick to D-Pad mapping
    if (ev.type == SDL_CONTROLLERAXISMOTION) {
        int value = ev.caxis.value;
        int stick = ev.caxis.which;
        int axis = ev.caxis.axis;
        int direction = (value < 0) ? -1 : 1;
        if (abs(value) >= CONTROL_STICK_PRESS_THRESHOLD) {
            ControlStickMovement movement = { stick, axis, direction };
            if (stickBindings.find(movement) != stickBindings.end()) {
                SDL_GameControllerButton btn = stickBindings[movement];
                if (!buttons_down[btn]) { buttons_pressed[btn] = true; }
                buttons_down[btn] = true;
                buttons_released[btn] = false;
            }
        }
        else {
            for (auto const& binding : stickBindings) {
                if (binding.first.stick == stick && binding.first.axis == axis) {
                    SDL_GameControllerButton btn = binding.second;
                    buttons_pressed[btn] = false;
                    buttons_down[btn] = false;
                    buttons_released[btn] = true;
                }
            }
        }
    }
}

// Map a keyboard key to a controller button for the purpose of handling input
void Controller::bindKeyToButton(SDL_Keycode from, SDL_GameControllerButton to) {
    bindings[from] = to;
}

// Map a control stick press on a particular axis to a controller button for the purpose of handling input
void Controller::bindStickToButton(int from_stick, int from_axis, int from_direction, SDL_GameControllerButton to) {
    ControlStickMovement movement = { from_stick, from_axis, (from_direction < 0) ? -1 : 1 };
    stickBindings[movement] = to;
}


bool Controller::getPressed(SDL_GameControllerButton b) { return buttons_pressed[b]; }
bool Controller::getButton(SDL_GameControllerButton b) { return buttons_down[b]; }
bool Controller::getReleased(SDL_GameControllerButton b) { return buttons_released[b]; }
