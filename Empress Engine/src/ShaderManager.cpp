#include "ShaderManager.h"
#include "Logger.h"
#include "Utility.h"

#include <algorithm>

ShaderManager::ShaderManager() {};

ShaderManager::~ShaderManager() {};

// Compile a GL Shader from source code string, and return the GLUint handle for it
GLuint ShaderManager::compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        LOG_ERROR("Error compiling shader: \n", infoLog);
    }
    return shader;
}

// Using the above compileShader function, creates a shader program using the vertex and fragment shader source code strings.
// Returns a GLuint handle for the program
GLuint ShaderManager::createShaderProgram(const char* identifier, const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    int success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        LOG_ERROR("Error linking shader: \n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    CompiledShaderProgram newprogram = { identifier, shaderProgram, false, 0, "", "" };

    if (myShaderPrograms.find(identifier) != myShaderPrograms.end()) {
        glDeleteShader(myShaderPrograms[identifier].shaderID);
    }
    myShaderPrograms[identifier] = newprogram;

    return shaderProgram;
}

// Similar as the above function, but instead of passing in source code, pass in filenames for files containing the source code
GLuint ShaderManager::createShaderFromSourceFiles(const char* identifier, const char* vertexShaderSourceFile, const char* fragmentShaderSourceFile) {
    std::string vertexSource = readFileIntoString(vertexShaderSourceFile);
    std::string fragmentSource = readFileIntoString(fragmentShaderSourceFile);
    if (vertexSource.length() == 0) { 
        LOG_ERROR("Could not create shader because vertex shader source was blank: ", identifier);
        return -1;
    }
    if (fragmentSource.length() == 0) { 
        LOG_ERROR("Could not create shader because fragment shader source was blank: ", identifier);
        return -1;
    }
    GLuint newShader = createShaderProgram(identifier, vertexSource.c_str(), fragmentSource.c_str());
    uint64_t newtimestamp = std::max(getFileTimestamp(vertexShaderSourceFile), getFileTimestamp(fragmentShaderSourceFile));
    myShaderPrograms[identifier].builtFromFiles = true;    
    myShaderPrograms[identifier].timestamp = newtimestamp;
    myShaderPrograms[identifier].fragmentSourceFilePath = fragmentShaderSourceFile;
    myShaderPrograms[identifier].vertexSourceFilePath = vertexShaderSourceFile;
    return newShader;
}

void ShaderManager::deleteShaderProgram(const char* identifier) {
    if (myShaderPrograms.find(identifier) != myShaderPrograms.end()) {
        glDeleteShader(myShaderPrograms[identifier].shaderID);
        myShaderPrograms.erase(identifier);
        return;
    }
    LOG_ERROR("Could not delete shader because no shader with that identifier was loaded: ", identifier);
  
}

GLuint ShaderManager::getShaderProgram(const char* identifier) {
    if (myShaderPrograms.find(identifier) != myShaderPrograms.end()) {
        return myShaderPrograms[identifier].shaderID;
    }
    LOG_ERROR("A shader was requested that doesn't exist: ", identifier);
    return -1;
}

void ShaderManager::hotReload() {
    const char* replaceIdentifier = 0;
    const char* replaceSourceVertex = 0;
    const char* replaceSourceFragment = 0;
    for (auto& each : myShaderPrograms) {
        if (each.second.builtFromFiles) {
            uint64_t newtimestamp = std::max(getFileTimestamp(each.second.vertexSourceFilePath), getFileTimestamp(each.second.fragmentSourceFilePath));
            if (newtimestamp > each.second.timestamp) {
                replaceIdentifier = each.first;
                replaceSourceVertex = each.second.vertexSourceFilePath;
                replaceSourceFragment = each.second.fragmentSourceFilePath;
            }
        }
    }

    if (replaceIdentifier) {
        Logger::log(CONSOLE_COLOR_GREEN, "SHADER", "Hot reload triggered for shader program: ", replaceIdentifier);
        deleteShaderProgram(replaceIdentifier);
        createShaderFromSourceFiles(replaceIdentifier, replaceSourceVertex, replaceSourceFragment);
    }

}
