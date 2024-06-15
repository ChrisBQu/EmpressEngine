#version 430 core
out vec2 TexCoords;

uniform vec2 fontTexturePos;
uniform vec2 fontTextureSize;
uniform vec2 screenSize;

void main()
{
    float posx = ((fontTexturePos.x / screenSize.x) * 2.0) - 1.0;
    float posy = (((fontTexturePos.y / screenSize.y) * 2.0) - 1.0) * -1.0;
    vec2 pos = vec2(posx, posy);

    float width = fontTextureSize.x / screenSize.x;
    float height = fontTextureSize.y / screenSize.y;

    // Define the vertices of the quad (two triangles to form a rectangle)
    vec2 vertices[6] = vec2[](
        vec2(0.0,  0.0),          // Bottom-left
        vec2(0.0, -height),       // Top-left
        vec2(width, -height),     // Top-right
        vec2(0.0,  0.0),          // Bottom-left
        vec2(width, -height),     // Top-right
        vec2(width,  0.0)         // Bottom-right
    );

    // Define the texture coordinates
    vec2 texCoords[6] = vec2[](
        vec2(0.0, 0.0),           // Bottom-left
        vec2(0.0, 1.0),           // Top-left
        vec2(1.0, 1.0),           // Top-right
        vec2(0.0, 0.0),           // Bottom-left
        vec2(1.0, 1.0),           // Top-right
        vec2(1.0, 0.0)            // Bottom-right
    );

    // Set the texture coordinates
    TexCoords = texCoords[gl_VertexID];

    // Offset the vertex position by the base position
    vec2 vertexPos = vertices[gl_VertexID] + pos;

    gl_Position = vec4(vertexPos, 0.0, 1.0);
}
