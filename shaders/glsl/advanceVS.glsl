#version 140

attribute vec3 position;
attribute vec2 texCoords;

out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	TexCoords = texCoords;
	gl_Position = projection * view * model * vec4(position, 1.0f);
}