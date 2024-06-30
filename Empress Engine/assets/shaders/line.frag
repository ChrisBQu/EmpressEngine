#version 430 core

in vec4 fragColor; // Input from vertex shader

out vec4 FragColor; // Final output color

void main() {
    FragColor = fragColor;
}
