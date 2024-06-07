#include "GL_Renderer.h"

#include "GameConfig.h"
#include "RenderInterface.h"
#include "TextureManager.h"
#include "Utility.h"
#include "Logger.h"

#include <iostream>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct GL_Context {
    GLuint VAO;
    GLuint textureID;
    GLuint transformSBOID;
    GLuint screenSizeID;
    GLuint orthoProjectionID;
    TextureManager textureManager;
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

void swapTexture(const char* identifier) {
    LoadedTexture loadedtexture = glcontext.textureManager.getTexture(identifier);
    glGenTextures(1, &glcontext.textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, glcontext.textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, loadedtexture.width, loadedtexture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, loadedtexture.data);
}

void initGLRenderer() {

    createShaders();
    glGenVertexArrays(1, &glcontext.VAO);
    glBindVertexArray(glcontext.VAO);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glDisable(0x809D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);

    glcontext.textureManager.loadTexture("tex0", "assets/textures/texture0.png");

    glGenBuffers(1, &glcontext.transformSBOID);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, glcontext.transformSBOID);


    // Get uniform handles
    glcontext.screenSizeID = glGetUniformLocation(SHADER_PROGRAM_DEFAULT, "screenSize");
    glcontext.orthoProjectionID = glGetUniformLocation(SHADER_PROGRAM_DEFAULT, "orthoProjection");

}

void glRender() {

    glcontext.textureManager.hotReload();

    swapTexture("tex0");


    glUseProgram(SHADER_PROGRAM_DEFAULT);
    glClearColor(SCREEN_CLEAR_COLOR[0], SCREEN_CLEAR_COLOR[1], SCREEN_CLEAR_COLOR[2], SCREEN_CLEAR_COLOR[3]);
    glClearDepth(0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);

    
    // Pass in the gameCamera to render things in the camera's view
    OrthographicCamera cam = renderData.gameCamera;
    glm::mat4 orthoProjection = makeOrthogrpahicProjectionMatrix(
        cam.pos.x - cam.dimensions.x / 2.0,
        cam.pos.x + cam.dimensions.x / 2.0,
        cam.pos.y - cam.dimensions.y / 2.0,
        cam.pos.y + cam.dimensions.y / 2.0
    ); 
    glUniformMatrix4fv(glcontext.orthoProjectionID, 1, GL_FALSE, glm::value_ptr(orthoProjection));

    // Render the transforms
    if (renderData.transforms.size() > 0) {
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(RenderTransform) * renderData.transforms.size(), &renderData.transforms[0], GL_DYNAMIC_DRAW);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, renderData.transforms.size());
        renderData.transforms.clear();
    }

    /*
    // Pass in the UI camera to render thing directly on the screen space coordinates
    OrthographicCamera uicam = renderData.uiCamera;
    glm::mat4 orthoProjection2 = makeOrthogrpahicProjectionMatrix(
        uicam.pos.x,
        uicam.dimensions.x,
        uicam.pos.y,
        uicam.dimensions.y
    );
    glUniformMatrix4fv(glcontext.orthoProjectionID, 1, GL_FALSE, glm::value_ptr(orthoProjection2));
    */
  
}
