#version 430 core
out vec4 FragColor;

in vec2 TexCoords;
in vec4 passThroughColor;

uniform sampler2D screenTexture;

void main()
{
    vec4 color = texture(screenTexture, TexCoords);

    if (color.a == 0.0) { discard; }

    if (color.r == 1.0 && color.g == 1.0 && color.b == 1.0) {
        color.r = passThroughColor[0];
        color.g = passThroughColor[1];
        color.b = passThroughColor[2];
    }

    FragColor = color;
}