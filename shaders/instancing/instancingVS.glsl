#version 140

attribute vec2 position;
attribute vec3 color;
attribute vec2 offset;

out vec3 Color;

void main() {
	Color = color;
	gl_Position = vec4(position + offset, 0.0f, 1.0f);
}