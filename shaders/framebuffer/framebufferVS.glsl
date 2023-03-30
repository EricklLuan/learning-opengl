#version 140

attribute vec2 position;
attribute vec2 texCoords;

out vec2 TexCoords;

void main() {
	TexCoords = texCoords;
	gl_Position = vec4(position.xy, 0.0, 1.0);
}