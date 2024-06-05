#version 430 core

layout (location = 0) in vec2 textureCoordsIn;

layout (location = 0) out vec4 fragColor;

layout (location = 0) uniform sampler2D textureAtlas;

uniform float palette[24];

void main() {

	vec4 textureColor = texelFetch(textureAtlas, ivec2(textureCoordsIn), 0);

	if (textureColor.a == 0.0) { discard; }
	
	int index = 0;

	if (textureColor.r == 0.0 && textureColor.g == 0.0 && textureColor.b == 0.0) { index = 0; }
	if (textureColor.r == 1.0 && textureColor.g == 1.0 && textureColor.b == 1.0) { index = 1; }

	if (textureColor.r == 0.0 && textureColor.g == 0.0 && textureColor.b == 1.0) { index = 2; }
	if (textureColor.r == 0.0 && textureColor.g == 1.0 && textureColor.b == 0.0) { index = 3; }
	if (textureColor.r == 1.0 && textureColor.g == 0.0 && textureColor.b == 0.0) { index = 4; }

	if (textureColor.r == 0.0 && textureColor.g == 1.0 && textureColor.b == 1.0) { index = 5; }
	if (textureColor.r == 1.0 && textureColor.g == 1.0 && textureColor.b == 0.0) { index = 6; }
	if (textureColor.r == 1.0 && textureColor.g == 0.0 && textureColor.b == 1.0) { index = 7; }

	textureColor.r = palette[index * 3] / 255.0;
	textureColor.g = palette[index * 3 + 1] / 255.0;
	textureColor.b = palette[index * 3 + 1] / 255.0;

	textureColor.r = pow(textureColor.r, 2.2);
	textureColor.g = pow(textureColor.g, 2.2);
	textureColor.b = pow(textureColor.b, 2.2);

	fragColor = textureColor;
}