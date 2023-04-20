#version 140

attribute vec3 aPosition;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
	gl_Position = projection * view * model * vec4(aPosition, 1.0f);
}