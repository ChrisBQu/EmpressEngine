#version 430 core

struct Transform {
	ivec2 atlasOffset;
	ivec2 spriteSize;
	vec2 pos;
	vec2 size;
	float depth;
	float rotation;
};

// Input

layout(std430, binding = 0) buffer TransformSBO {
	Transform transforms[];
};

uniform mat4 orthoProjection;

// Output

layout (location = 0) out vec2 textureCoordsOut;

void main() {

	Transform transform = transforms[gl_InstanceID];

	vec2 vertices[6] = {
		transform.pos,
		vec2(transform.pos + vec2(0.0, transform.size.y)),
		vec2(transform.pos + vec2(transform.size.x, 0.0)),
		vec2(transform.pos + vec2(transform.size.x, 0.0)),
		vec2(transform.pos + vec2(0.0, transform.size.y)),
		transform.pos + transform.size
	};

	float left = transform.atlasOffset.x;
	float top = transform.atlasOffset.y;
	float right = transform.atlasOffset.x + transform.spriteSize.x;
	float bottom = transform.atlasOffset.y + transform.spriteSize.y;

	vec2 textureCoords[6] = {
		vec2(left, top),
		vec2(left, bottom),
		vec2(right, top),
		vec2(right, top),
		vec2(left, bottom),
		vec2(right, bottom)
	};

	float radians = radians(transform.rotation);
    float cosTheta = cos(radians);
    float sinTheta = sin(radians);
    mat4 rotationMatrix = mat4(
        cosTheta, -sinTheta, 0.0, 0.0,
        sinTheta,  cosTheta, 0.0, 0.0,
        0.0,      0.0,      1.0, 0.0,
        0.0,      0.0,      0.0, 1.0
    );

	// Normalize to screen coordinates
	vec2 vertexPos = vertices[gl_VertexID];
	gl_Position = orthoProjection * rotationMatrix * vec4(vertexPos, transform.depth, 1.0);

	textureCoordsOut = textureCoords[gl_VertexID];

}