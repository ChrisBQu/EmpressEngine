#include "GL_Renderer.h"

#include "GameConfig.h"
#include "RenderInterface.h"
#include "Utility.h"
#include "Logger.h"

#include <glm/gtc/type_ptr.hpp>

glm::mat4 makeOrthographicProjectionMatrix(float left, float right, float top, float bottom) {
    glm::mat4 result(1.0f);
    float near = -1000.0f;
    float far = 1000.0f;
    result[0][0] = 2.0f / (right - left);
    result[1][1] = 2.0f / (top - bottom);
    result[2][2] = 2.0f / (near - far);
    result[3][0] = -(right + left) / (right - left);
    result[3][1] = -(top + bottom) / (top - bottom);
    result[3][2] = (far + near) / (near - far);
    result[3][3] = 1.0f;
    return result;
}

// Constructor
GL_Renderer::GL_Renderer() {}

void GL_Renderer::swapTexture(const char* identifier) {
    LoadedTexture loadedtexture = textureManager.getTexture(identifier);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, glcontext.textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, loadedtexture.width, loadedtexture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, loadedtexture.data);
}

int GL_Renderer::init() {

    shaderManager.buildShadersFromJSONList(SHADER_MANIFEST_FILEPATH);

    glGenVertexArrays(1, &glcontext.VAO);
    glBindVertexArray(glcontext.VAO);
    glEnable(GL_FRAMEBUFFER_SRGB);
    glDisable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_GREATER);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    textureManager.loadTexture("tex0", "assets/textures/texture0.png");
    textureManager.loadTexture("tex1", "assets/textures/texture1.png");
    textureManager.loadTexture("tex2", "assets/textures/texture2.png");

    glGenTextures(1, &glcontext.textureID);

    glGenBuffers(1, &glcontext.transformSBOID);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, glcontext.transformSBOID);

    glGenFramebuffers(1, &glcontext.FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, glcontext.FBO);

    // Init post-processing
    glGenTextures(1, &glcontext.postProcessingTextureBuffer);
    glBindTexture(GL_TEXTURE_2D, glcontext.postProcessingTextureBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gameConfig.screenWidth, gameConfig.screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, glcontext.postProcessingTextureBuffer, 0);
    glGenRenderbuffers(1, &glcontext.postProcessingDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, glcontext.postProcessingDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, gameConfig.screenWidth, gameConfig.screenHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, glcontext.postProcessingDepthBuffer);

    // Init text rendering
    fontManager.buildFontList(FONT_MANIFEST_FILEPATH);
    glGenVertexArrays(1, &glcontext.textVAO);
    glGenBuffers(1, &glcontext.textVBO);
    glBindVertexArray(glcontext.textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, glcontext.textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return 0;
}

void GL_Renderer::resize(int width, int height) {
    gameConfig.screenWidth = width;
    gameConfig.screenHeight = height;

    // Update post-processing texture buffer
    glBindTexture(GL_TEXTURE_2D, glcontext.postProcessingTextureBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Update post-processing depth buffer
    glBindRenderbuffer(GL_RENDERBUFFER, glcontext.postProcessingDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void GL_Renderer::drawText(std::string shader_identifier, std::string font_identifier, std::string text, glm::vec2 pos, glm::vec2 scale, glm::vec4 color) {
    float xs = (float)gameConfig.screenWidth / (float)DEFAULT_SCREEN_WIDTH;
    float ys = (float)gameConfig.screenHeight / (float)DEFAULT_SCREEN_HEIGHT;

    GLuint shaderProgram = shaderManager.getShaderProgram(shader_identifier);
    glUseProgram(shaderProgram);
    GLuint textColorLocation = glGetUniformLocation(shaderProgram, "textColor");
    GLuint projectionLocation = glGetUniformLocation(shaderProgram, "projection");
    glUniform4f(textColorLocation, color[0], color[1], color[2], color[3]);
    glm::mat4 projection = glm::ortho(0.0f, (float)gameConfig.screenWidth, 0.0f, (float)gameConfig.screenHeight);
    glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(glcontext.textVAO);

    float scaledPosX = pos[0] * xs;
    float scaledPosY = pos[1] * ys;

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) {
        Character ch = fontManager.getFont(font_identifier)[*c];
        if (ch.textureID == 0) {
            LOG_ERROR("Character texture not loaded for character: ", std::string(1, *c));
            continue;
        }

        float xpos = scaledPosX + ch.bearing.x * xs;
        float ypos = scaledPosY - (ch.size.y - ch.bearing.y) * ys;
        float w = ch.size.x * xs;
        float h = ch.size.y * ys;

        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        glBindTexture(GL_TEXTURE_2D, ch.textureID);
        glBindBuffer(GL_ARRAY_BUFFER, glcontext.textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        scaledPosX += (ch.advance >> 6) * xs;
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}




void GL_Renderer::render() {
    if (HOT_TEXTURE_SWAPPING_ENABLED) { textureManager.hotReload(); }
    if (HOT_SHADER_SWAPPING_ENABLED) { shaderManager.hotReload(); }

    // Prepare for rendering to FBO
    glBindVertexArray(glcontext.VAO);
    glBindFramebuffer(GL_FRAMEBUFFER, glcontext.FBO);
    glClearColor(SCREEN_CLEAR_COLOR[0], SCREEN_CLEAR_COLOR[1], SCREEN_CLEAR_COLOR[2], SCREEN_CLEAR_COLOR[3]);
    glClearDepth(1.0f); // Depth clear value should be 1.0f for the farthest depth
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, gameConfig.screenWidth, gameConfig.screenHeight);

    glUseProgram(shaderManager.getShaderProgram("DEFAULT_QUAD"));


    // Pass in the gameCamera to render things in the camera's view
    OrthographicCamera cam = renderData.gameCamera;
    glm::mat4 orthoProjection = makeOrthographicProjectionMatrix(
        cam.pos.x - cam.dimensions.x / 2.0,
        cam.pos.x + cam.dimensions.x / 2.0,
        cam.pos.y - cam.dimensions.y / 2.0,
        cam.pos.y + cam.dimensions.y / 2.0
    );

    // Render the sprites to the FBO
    glBindFramebuffer(GL_FRAMEBUFFER, glcontext.FBO);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glcontext.orthoProjectionMatrixID = glGetUniformLocation(shaderManager.getShaderProgram("DEFAULT_QUAD"), "orthoProjection");
    glUniformMatrix4fv(glcontext.orthoProjectionMatrixID, 1, GL_FALSE, glm::value_ptr(orthoProjection));
    for (const auto& it : renderData.transforms) {
        swapTexture(it.first.c_str());
        if (it.second.size() > 0) {
            glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(RenderTransform) * it.second.size(), &it.second[0], GL_DYNAMIC_DRAW);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, it.second.size());
        }
    }
    renderData.transforms.clear();

    // Post-Processing Steps
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderManager.getShaderProgram("DEFAULT_POST"));
    glBindTexture(GL_TEXTURE_2D, glcontext.postProcessingTextureBuffer);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Draw text
    
    glm::mat4 projection = glm::ortho(0.0f, (float)gameConfig.screenWidth, 0.0f, (float)gameConfig.screenHeight);
    GLuint textOrthoHandle = glGetUniformLocation(shaderManager.getShaderProgram("FONT_SHADER"), "projection");
    glUniformMatrix4fv(textOrthoHandle, 1, GL_FALSE, glm::value_ptr(projection));
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for (TextRenderItem& each_text : renderData.textItems) {
        drawText("FONT_SHADER", each_text.fontIdentifier, each_text.text, each_text.pos, each_text.scale, each_text.color);
    }
    renderData.textItems.clear();
    

}
