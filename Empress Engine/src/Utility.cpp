#include "Utility.h"
#include "Logger.h"

#include <fstream>
#include <sstream>
#include <iostream>

std::string readFileIntoString(const char* filename) {
	std::fstream fp(filename);
    if (!fp.is_open()) {
        LOG_ERROR("Could not open file: ", filename);
        return "";
    }
	std::stringstream buffer;
	buffer << fp.rdbuf();
    if (fp.fail()) {
        LOG_ERROR("Failed reading file: ", filename);
        return "";
    }
	return buffer.str();
}

uint64_t getFileTimestamp(const char* filename) {
    struct stat fileInfo;
    if (stat(filename, &fileInfo) != 0) {
        LOG_ERROR("Could not get file attributes for: ", filename);
        return 0;
    }
    return (uint64_t)fileInfo.st_mtime;
}