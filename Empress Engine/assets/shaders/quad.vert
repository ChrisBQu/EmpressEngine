#version 430 core

struct Transform {
    ivec2 atlasOffset;      // Size: 8
    ivec2 spriteSize;       // Size: 8 
    vec2 pos;               // Size: 8
    vec2 size;              // Size: 8 
    float depth;            // Size: 4
    float rotation;         // Size: 4
    float alpha;            // Size: 4
    float padding;          // Size: 4
};

// Input

layout(std430, binding = 0) buffer TransformSBO {
    Transform transforms[];
};

uniform mat4 orthoProjection;

// Output

layout (location = 0) out vec2 textureCoordsOut;
layout (location = 1) out float alphaOut;

void main() {

    Transform transform = transforms[gl_InstanceID];

    vec2 halfSize = 0.5 * transform.size;

    // Define vertices relative to the origin (0,0)
    vec2 vertices[6] = {
        vec2(-halfSize.x, -halfSize.y),
        vec2(-halfSize.x,  halfSize.y),
        vec2( halfSize.x, -halfSize.y),
        vec2( halfSize.x, -halfSize.y),
        vec2(-halfSize.x,  halfSize.y),
        vec2( halfSize.x,  halfSize.y)
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

    // Apply rotation and translation
    vec2 vertexPos = (rotationMatrix * vec4(vertices[gl_VertexID], 0.0, 1.0)).xy + transform.pos;

    // Normalize to screen coordinates
    gl_Position = orthoProjection * vec4(vertexPos, 1.0 - transform.depth, 1.0);

    textureCoordsOut = textureCoords[gl_VertexID];
    alphaOut = transform.alpha;
}
