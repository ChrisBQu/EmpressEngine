#include "GL_Renderer.h"

#include "GameConfig.h"
#include "RenderInterface.h"
#include "Utility.h"
#include "Logger.h"

#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "3rdParty/stb_image.h"

struct GL_Context {
    GLuint textureID;
    GLuint transformSBOID;
    GLuint screenSizeID;
    GLuint orthoProjectionID;
};

GL_Context glcontext;

// List of shaders
GLuint SHADER_PROGRAM_DEFAULT;
GLuint SHADER_PROGRAM_BLANK;

// Compile a GL Shader from source code string, and return the GLUint handle for it
GLuint compileShader(GLenum type, const char* source) {
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
GLuint createShaderProgram(const char* vertexSource, const char* fragmentSource) {
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
    return shaderProgram;
}

// Similar as the above function, but instead of passing in source code, pass in filenames for files containing the source code
GLuint createShaderFromSourceFiles(const char* vertexShaderSourceFile, const char* fragmentShaderSourceFile) {
    std::string vertexSource = readFileIntoString(vertexShaderSourceFile);
    std::string fragmentSource = readFileIntoString(fragmentShaderSourceFile);
    GLuint newShader = createShaderProgram(vertexSource.c_str(), fragmentSource.c_str());
    return newShader;
}

// Call once to initialize all of the shaders
void createShaders() {
    SHADER_PROGRAM_DEFAULT = createShaderFromSourceFiles("shaders/quad.vert", "shaders/default.frag");
    SHADER_PROGRAM_BLANK = createShaderFromSourceFiles("shaders/quad.vert", "shaders/blank.frag");
}

void initGLRenderer() {
    createShaders();
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glDisable(0x809D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);
    const char* TEXTURE_PATH = "assets/textures/texture0.png";
    int width, height, channels;
    char* data = (char*)stbi_load(TEXTURE_PATH, &width, &height, &channels, 4);
    if (!data) { LOG_ERROR("Failed to load texture: ", TEXTURE_PATH); }

    glGenTextures(1, &glcontext.textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, glcontext.textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    glGenBuffers(1, &glcontext.transformSBOID);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, glcontext.transformSBOID);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(RenderTransform) * MAX_QUADS_ON_SCREEN, renderData.transforms, GL_DYNAMIC_DRAW);

    // Get uniform handles
    glcontext.screenSizeID = glGetUniformLocation(SHADER_PROGRAM_DEFAULT, "screenSize");
    glcontext.orthoProjectionID = glGetUniformLocation(SHADER_PROGRAM_DEFAULT, "orthoProjection");

}

void glRender() {
    glUseProgram(SHADER_PROGRAM_DEFAULT);
    glClearColor(SCREEN_CLEAR_COLOR[0], SCREEN_CLEAR_COLOR[1], SCREEN_CLEAR_COLOR[2], SCREEN_CLEAR_COLOR[3]);
    glClearDepth(0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);

    // Pass in uniforms
   
    // glm::vec2 screenSize = { (float)DEFAULT_SCREEN_WIDTH, (float)DEFAULT_SCREEN_HEIGHT };
    // glUniform2fv(glcontext.screenSizeID, 1, &screenSize.x);

    OrthographicCamera cam = renderData.gameCamera;
    glm::mat4 orthoProjection = makeOrthogrpahicProjectionMatrix(
        cam.pos.x - cam.dimensions.x / 2.0,
        cam.pos.x + cam.dimensions.x / 2.0,
        cam.pos.y - cam.dimensions.y / 2.0,
        cam.pos.y + cam.dimensions.y / 2.0
    );    
    glUniformMatrix4fv(glcontext.orthoProjectionID, 1, GL_FALSE, glm::value_ptr(orthoProjection));

    // End passing in uniforms

    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(RenderTransform) * renderData.transformCount, renderData.transforms);
    glDrawArraysInstanced(GL_TRIANGLES, 0, 6, renderData.transformCount);
    renderData.transformCount = 0;
  
}
