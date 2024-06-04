#include "Utility.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string readFileIntoString(const char* filename) {
	std::fstream fp(filename);
	std::stringstream buffer;
	buffer << fp.rdbuf();
	return buffer.str();
}
