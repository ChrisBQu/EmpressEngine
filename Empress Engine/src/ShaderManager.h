#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

#include <GL/glew.h>

#include <chrono>
#include <cstdint>
#include <map>
#include <thread>

struct CompiledShaderProgram {
	const char* identifier;
	GLuint shaderID;

	// Shader metadata for hot reload
	bool builtFromFiles;
	uint64_t timestamp;
	const char* vertexSourceFilePath;
	const char* fragmentSourceFilePath;
};

class ShaderManager {
public:
	ShaderManager();
	~ShaderManager();
	GLuint compileShader(GLenum type, const char* source);
	GLuint createShaderProgram(const char* identifier, const char* vertexSource, const char* fragmentSource);
	GLuint createShaderFromSourceFiles(const char* identifier, const char* vertexShaderSourceFile, const char* fragmentShaderSourceFile);
	GLuint getShaderProgram(const char* identifier);
	void deleteShaderProgram(const char* identifier);
	void hotReload();
private:
	std::map<const char*, CompiledShaderProgram> myShaderPrograms;
};

#endif // SHADER_MANAGER_H