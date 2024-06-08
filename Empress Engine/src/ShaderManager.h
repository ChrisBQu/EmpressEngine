#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <GL/glew.h>

#include <chrono>
#include <cstdint>
#include <map>
#include <string>
#include <thread>

// Structure is used internally to hold loaded shader programs in such a way that hot reloading, and mapping to a string identifier can be supported,
struct CompiledShaderProgram {
	std::string identifier;
	GLuint shaderID;

	// Shader metadata for hot reload
	bool builtFromFiles;
	uint64_t timestamp;
	std::string vertexSourceFilePath;
	std::string fragmentSourceFilePath;
};

class ShaderManager {
public:
	ShaderManager();
	~ShaderManager();
	GLuint compileShader(GLenum type, const char* source);
	GLuint createShaderProgram(std::string identifier, const char* vertexSource, const char* fragmentSource);
	GLuint createShaderFromSourceFiles(std::string identifier, const char* vertexShaderSourceFile, const char* fragmentShaderSourceFile);
	int buildShadersFromJSONList(const char* json_file);
	GLuint getShaderProgram(std::string identifier);
	void deleteShaderProgram(std::string identifier);
	void hotReload();
private:
	std::map<std::string, CompiledShaderProgram> myShaderPrograms;
};

#endif // SHADER_MANAGER_H