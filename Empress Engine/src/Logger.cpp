#include "Logger.h"
#include <iostream>

const char* CONSOLE_TEXT_COLOR_CODES[CONSOLE_COLOR_COLOR_COUNT] = {
	"\033[0m",						// CONSOLE_COLOR_DEFAULT
	"\033[91m",						// CONSOLE_COLOR_RED
	"\033[92m",						// CONSOLE_COLOR_GREEN
	"\033[93m",						// CONSOLE_COLOR_YELLOW
	"\033[94m"						// CONSOLE_COLOR_BLUE
};
