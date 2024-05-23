#version 460
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 tex_coord;
layout (location = 3) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord;
out vec3 fragPos;
out vec3 scaledNormal;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0);
	texCoord = vec2(tex_coord.x, 1 - tex_coord.y);
	scaledNormal = normal;
	fragPos = vec3(model * vec4(position, 1.0));
}