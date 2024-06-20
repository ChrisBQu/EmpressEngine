#version 430 core
in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec4 textColor;

void main()
{    
    float threshold = .003; // Adjust this value as needed
    float alpha = step(threshold, texture(text, TexCoords).r);
    vec4 sampled = vec4(1.0, 1.0, 1.0, alpha);
    color = textColor * sampled;
}