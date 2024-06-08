#include "ShaderManager.h"
#include "Logger.h"
#include "Utility.h"

#include "3rdParty/json.hpp"

#include <algorithm>

// Constructor
ShaderManager::ShaderManager() {};

// Destructor
ShaderManager::~ShaderManager() {
    for (auto& each : myShaderPrograms) {
        glDeleteShader(each.second.shaderID);
    }
};

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
GLuint ShaderManager::createShaderProgram(std::string identifier, const char* vertexSource, const char* fragmentSource) {
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
GLuint ShaderManager::createShaderFromSourceFiles(std::string identifier, const char* vertexShaderSourceFile, const char* fragmentShaderSourceFile) {
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

// Takes a JSON file, and builds all of the shaders defined in it. The format should be:
// {
//      "SHADER1" : {
//          "vertexSource": "shaders/source.vert",
//          "fragmentSource" : "shaders/source.frag"
//      },
//      "SHADER2" : {
//          "vertexSource": "shaders/source.vert",
//          "fragmentSource" : "shaders/source.frag"
//      }
//
// }
int ShaderManager::buildShadersFromJSONList(const char* json_file) {
    std::string s = readFileIntoString(json_file);
    if (s.length() == 0) {
        LOG_ERROR("Could not build shader programs because JSON file could not be read.");
        return 1;
    }
    if (!nlohmann::json::accept(s)) {
        LOG_ERROR("Could not build shader programs because JSON is malformed.");
        return 1;
    }
    nlohmann::json data = nlohmann::json::parse(s);
    for (auto it = data.begin(); it != data.end(); it++) {
        std::string vertSource = it.value()["vertexSource"];
        std::string fragSource = it.value()["fragmentSource"];
        std::string identifier = it.key();
        createShaderFromSourceFiles(identifier, vertSource.c_str(), fragSource.c_str());

    }
}

// Unload a shader by identifier, and remove it from the list of shader programs
void ShaderManager::deleteShaderProgram(std::string identifier) {
    if (myShaderPrograms.find(identifier) != myShaderPrograms.end()) {
        glDeleteShader(myShaderPrograms[identifier].shaderID);
        myShaderPrograms.erase(identifier);
        return;
    }
    LOG_ERROR("Could not delete shader because no shader with that identifier was loaded: ", identifier);
  
}

// Get shader program by identifier (GLuint used by OpenGL)
GLuint ShaderManager::getShaderProgram(std::string identifier) {
    if (myShaderPrograms.find(identifier) != myShaderPrograms.end()) {
        return myShaderPrograms[identifier].shaderID;
    }
    LOG_ERROR("A shader was requested that doesn't exist: ", identifier);
    return -1;
}

// Call once per frame to allow changes to shader file source codes to automatically result in recompiling/linking of the shader programs
void ShaderManager::hotReload() {

    std::vector<std::string> replaceIdentifiers;
    std::vector<std::string> replaceVertexSources;
    std::vector<std::string> replaceFragSources;

    // Loop through the array and find all of the shaders that need to be changed. This is to avoid modifying a map that we are iterating through
    for (auto& each : myShaderPrograms) {
        if (each.second.builtFromFiles) {
            uint64_t newtimestamp = std::max(getFileTimestamp(each.second.vertexSourceFilePath.c_str()), getFileTimestamp(each.second.fragmentSourceFilePath.c_str()));
            if (newtimestamp > each.second.timestamp) {
                replaceIdentifiers.push_back(each.first);
                replaceVertexSources.push_back(each.second.vertexSourceFilePath);
                replaceFragSources.push_back(each.second.fragmentSourceFilePath);
            }
        }
    }

    

    // Reload all of the shaders that need to be reloaded
    for (size_t i = 0; i < replaceIdentifiers.size(); i++) {
        Logger::log(CONSOLE_COLOR_GREEN, "SHADER", "Hot reload triggered for shader program: ", replaceIdentifiers[i]);
        deleteShaderProgram(replaceIdentifiers[i]);
        createShaderFromSourceFiles(replaceIdentifiers[i], replaceVertexSources[i].c_str(), replaceFragSources[i].c_str());

    }

}
