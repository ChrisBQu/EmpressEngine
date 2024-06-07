#include "GL_Renderer.h"

#include "GameConfig.h"
#include "RenderInterface.h"
#include "ShaderManager.h"
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
    ShaderManager shaderManager;
    TextureManager textureManager;
};

GL_Context glcontext;

// Call once to initialize all of the shaders
void createShaders() {
    glcontext.shaderManager.createShaderFromSourceFiles("DEFAULT", "shaders/quad.vert", "shaders/default.frag");
    glcontext.shaderManager.createShaderFromSourceFiles("BLANK", "shaders/quad.vert", "shaders/blank.frag");
}

void swapTexture(const char* identifier) {
    LoadedTexture loadedtexture = glcontext.textureManager.getTexture(identifier);
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
    glDisable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);

    glcontext.textureManager.loadTexture("tex0", "assets/textures/texture0.png");
    glGenTextures(1, &glcontext.textureID);

    glGenBuffers(1, &glcontext.transformSBOID);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, glcontext.transformSBOID);

    // Get uniform handles
    glcontext.screenSizeID = glGetUniformLocation(glcontext.shaderManager.getShaderProgram("DEFAULT"), "screenSize");
    glcontext.orthoProjectionID = glGetUniformLocation(glcontext.shaderManager.getShaderProgram("DEFAULT"), "orthoProjection");

}

void glRender() {

    glcontext.textureManager.hotReload();
    glcontext.shaderManager.hotReload();
    swapTexture("tex0");


    glUseProgram(glcontext.shaderManager.getShaderProgram("DEFAULT"));
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
