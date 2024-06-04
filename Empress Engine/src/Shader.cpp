#include "Shader.h"
#include "SDL_image.h"
#include "Utility.h"

#include <iostream>
#include <fstream>

glm::mat4 orthoProjection;

// List of shaders
GLuint SHADER_PROGRAM_DEFAULT;
GLuint SHADER_PROGRAM_RIPPLE;


// Create an orthographic projection matrix
void setupOrthoProjection(unsigned int screenWidth, unsigned int screenHeight) { 
    orthoProjection = glm::ortho(0.0f, (float)screenWidth, (float)screenHeight, 0.0f, -1.0f, 1.0f); 
}

// Create a quad that can be rendered to, and return the vertex array object for it
GLuint generateQuadVAO() {
    float quadVertices[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,  // Top-left
        -1.0f, -1.0f,  0.0f, 0.0f,  // Bottom-left
         1.0f, -1.0f,  1.0f, 0.0f,  // Bottom-right
        -1.0f,  1.0f,  0.0f, 1.0f,  // Top-left
         1.0f, -1.0f,  1.0f, 0.0f,  // Bottom-right
         1.0f,  1.0f,  1.0f, 1.0f   // Top-right
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

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
        std::cout << "Error compiling shader: " << infoLog << std::endl;
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
        std::cout << "Error linking shader program: " << infoLog << std::endl;
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
    SHADER_PROGRAM_DEFAULT = createShaderFromSourceFiles("shaders/projection.vert", "shaders/projection.frag");
    SHADER_PROGRAM_RIPPLE = createShaderFromSourceFiles("shaders/simple.vert", "shaders/ripple.frag");
}

GLuint convertSDLTextureToGLTexture(SDL_Texture* imgTexture, SDL_Renderer* renderer) {

    // Get the size of the textur
    SDL_Point txsize;
    SDL_QueryTexture(imgTexture, NULL, NULL, &txsize.x, &txsize.y);

    // Create an intermediate SDL surface to hold the pixel data
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, txsize.x, txsize.y, 32, SDL_PIXELFORMAT_RGBA32);

    // Render the texture to the surface
    SDL_Texture* target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, txsize.x, txsize.y);
    SDL_SetRenderTarget(renderer, target);
    SDL_RenderCopy(renderer, imgTexture, NULL, NULL);
    SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA32, surface->pixels, surface->pitch);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_DestroyTexture(target);

    // Create an OpenGL texture from the surface
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Configure the OpenGL texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, txsize.x, txsize.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Free the intermediate SDL surface, and return the OpenGL texture ID
    SDL_FreeSurface(surface);
    return textureID;
}

// Render a texture onto a given quad vertex array object. It assumes that we are using screen coordinates for this.
// The position is in {X, Y} form. The size is in {W, H} form. And rotation is in degrees (turning clockwise.)
void renderTexture(GLuint textureID, GLuint quadVAO, const glm::vec2& screenPosition, const glm::vec2& screenSize, float rotation) {
    GLuint shaderProgram = SHADER_PROGRAM_DEFAULT;
    glUseProgram(shaderProgram);
    GLuint orthoMatrixLocation = glGetUniformLocation(SHADER_PROGRAM_DEFAULT, "projection");

    // Set the orthographic projection matrix
    glUniformMatrix4fv(orthoMatrixLocation, 1, GL_FALSE, glm::value_ptr(orthoProjection));

    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, glm::vec3(screenPosition.x, screenPosition.y, 0.0f));

    // Apply rotation around the center
    transform = glm::translate(transform, glm::vec3(screenPosition.x + screenSize.x / 2.0f, screenPosition.y + screenSize.y / 2.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::scale(transform, glm::vec3(screenSize.x / 2.0f, screenSize.y / 2.0f, 1.0f));

    // Pass the transformation matrix to the shader
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "transform"), 1, GL_FALSE, glm::value_ptr(transform));

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(shaderProgram, "screenTexture"), 0);

    // Bind the VAO and draw the quad
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

// Take a shader program, GL Texture ID, and a set of parameters for the shader program, and apply the shader to the texture
// A new texture ID will be generated and returned
GLuint applyShaderToTexture(GLuint shaderProgram, GLuint textureID, const std::vector<float> parameters) {

    // Get texture width and height
    int width, height;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

    // Create a new texture to store the result
    GLuint resultTexture;
    glGenTextures(1, &resultTexture);
    glBindTexture(GL_TEXTURE_2D, resultTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create a framebuffer object
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // Attach the new texture to the FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, resultTexture, 0);

    // Check if the framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer not complete" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return 0;
    }

    // Set the viewport to the size of the texture
    glViewport(0, 0, width, height);

    // Bind the shader program
    glUseProgram(shaderProgram);

    // Pass the parameters to the shader
    GLint paramLocation = glGetUniformLocation(shaderProgram, "parameters");
    if (paramLocation != -1) {
        glUniform1fv(paramLocation, parameters.size(), parameters.data());
    }

    // Set up the quad vertex data
    float quadVertices[] = {
        -1.0f,  1.0f,  0.0f, 1.0f,  // Top-left
        -1.0f, -1.0f,  0.0f, 0.0f,  // Bottom-left
         1.0f, -1.0f,  1.0f, 0.0f,  // Bottom-right
        -1.0f,  1.0f,  0.0f, 1.0f,  // Top-left
         1.0f, -1.0f,  1.0f, 0.0f,  // Bottom-right
         1.0f,  1.0f,  1.0f, 1.0f   // Top-right
    };

    GLuint quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Bind the original texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(shaderProgram, "screenTexture"), 0);

    // Render the quad
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Clean up
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteFramebuffers(1, &fbo);

    return resultTexture;
}

