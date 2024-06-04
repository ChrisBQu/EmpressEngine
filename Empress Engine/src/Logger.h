
#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

// Supported colors
enum ConsoleTextColor { 
	CONSOLE_COLOR_DEFAULT,
	CONSOLE_COLOR_RED,
	CONSOLE_COLOR_GREEN,
	CONSOLE_COLOR_YELLOW,
	CONSOLE_COLOR_BLUE,
	CONSOLE_COLOR_COLOR_COUNT 
};

extern const char* CONSOLE_TEXT_COLOR_CODES[];

class Logger {
public:
	template <typename... Args>
	static void log(ConsoleTextColor color, const char* tag, Args&&... args);

	template <typename... Args>
	static void log(const char* tag, Args&&... args);

	template <typename... Args>
	static void print(Args&&... args);

	template <typename... Args>
	static void print(ConsoleTextColor color, Args&&... args);
};

template<typename... Args>
void Logger::log(ConsoleTextColor color, const char* tag, Args&&... args) {
	std::cout << CONSOLE_TEXT_COLOR_CODES[color] << "[" << tag << "] ";
	((std::cout << std::forward<Args>(args)), ...);
	std::cout << CONSOLE_TEXT_COLOR_CODES[CONSOLE_COLOR_DEFAULT] << std::endl;
}

template<typename... Args>
void Logger::log(const char* tag, Args&&... args) { 
	Logger::log(CONSOLE_COLOR_DEFAULT, tag, args...); 
}

template<typename... Args>
void Logger::print(Args&&... args) { 
	((std::cout << std::forward<Args>(args)), ...);
	std::cout << std::endl;
}

template<typename... Args>
void Logger::print(ConsoleTextColor color, Args&&... args) {
	std::cout << CONSOLE_TEXT_COLOR_CODES[color];
	Logger::print(args...);
}

// Convenience macros

#define LOG(...) Logger::log(CONSOLE_COLOR_DEFAULT, "LOG",  __VA_ARGS__)
#define LOG_ERROR(...) Logger::log(CONSOLE_COLOR_RED, "ERROR",  __VA_ARGS__)
#define LOG_DEBUG(...) Logger::log(CONSOLE_COLOR_BLUE, "DEBUG", __VA_ARGS__)
#define LOG_WARNING(...) Logger::log(CONSOLE_COLOR_YELLOW, "WARNING", __VA_ARGS__)

#endif // LOGGER_H