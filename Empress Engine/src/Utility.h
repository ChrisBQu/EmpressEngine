#ifndef UTILITY_H
#define UTILITY_H
#include <string>

std::string readFileIntoString(const char* filename);
uint64_t getFileTimestamp(const char* filename);

float mapRange(int value, int fromMin, int fromMax, float toMin, float toMax);

#endif // UTILITY_H