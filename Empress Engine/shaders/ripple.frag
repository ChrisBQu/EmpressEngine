#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D screenTexture;
uniform float parameters[1]; // Array containing the time parameter

void main() {
    // Extract the time parameter from the array
    float time = parameters[0];
    
    // Create a waving effect by altering the texture coordinates based on a sine wave
    float wave = sin(TexCoord.y * 10.0 + time * 2.0) * 0.1;
    vec2 distortedTexCoord = vec2(TexCoord.x + wave, TexCoord.y);
    
    // Sample the texture using the distorted texture coordinates
    vec4 texColor = texture(screenTexture, distortedTexCoord);
    
    // Set the output color
    FragColor = texColor;
}
