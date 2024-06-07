#ifndef UTILITY_H
#define UTILITY_H
#include <string>

std::string readFileIntoString(const char* filename);
uint64_t getFileTimestamp(const char* filename);

#endif // UTILITY_H