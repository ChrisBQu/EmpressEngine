#version 430 core

struct Transform {
    vec2 point1;
    vec2 point2;
    vec2 point3;
    vec2 point4;
    vec4 color;
};

// Input

layout(std430, binding = 0) buffer TransformSBO {
    Transform transforms[];
};

uniform mat4 projectionMatrix; // Projection matrix passed from OpenGL

in vec2 inPosition; // Vertex position (used but not from SSBO, just passed in)

out vec4 fragColor; // Output fragment color

void main() {
    // Calculate the index based on the instance ID
    int instanceID = gl_InstanceID;
    
    // Access the instance data from the SSBO
    vec2 point1 = transforms[instanceID].point1;
    vec2 point2 = transforms[instanceID].point2;
    vec2 point3 = transforms[instanceID].point3;
    vec2 point4 = transforms[instanceID].point4;

    // Define vertices of the rectangle
    vec2 vertices[6] = vec2[](
        point1,
        point2,
        point3,
        point1,
        point3,
        point4
    );

    // Transform vertices by projection matrix
    vec4 transformedVertices[6];
    for (int i = 0; i < 6; ++i) {
        transformedVertices[i] = projectionMatrix * vec4(vertices[i], 0.0, 1.0);
    }

    // Output vertex positions (in clip space)
    gl_Position = transformedVertices[gl_VertexID];

    // Output color
    fragColor = transforms[instanceID].color;
}