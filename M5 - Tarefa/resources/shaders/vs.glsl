#version 460
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normalMatrix; // Transpose of the inverse of the model matrix

out vec2 texCoord;
out vec3 fragPos;
out vec3 transformedNormal;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    texCoord = vec2(tex_coord.x, 1 - tex_coord.y);
    transformedNormal = mat3(normalMatrix) * normal; // Transform normal correctly
    fragPos = vec3(model * vec4(position, 1.0));
}
