#version 140

attribute vec3 position;
attribute vec3 normal;

layout (std140) uniform Matrices {
	mat4 projection;
	mat4 view;
};

uniform mat4 model;

out vec3 Color;

void main() {
	Color = normal;
	gl_Position = projection * view * model * vec4(position, 1.0f);
}