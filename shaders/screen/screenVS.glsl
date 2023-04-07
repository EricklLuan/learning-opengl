#version 140

attribute vec2 aPosition;
attribute vec2 aTexCoords;

out vec2 TexCoords;

void main() {
	TexCoords = aTexCoords;
	gl_Position = vec4(aPosition, 0.0f, 1.0f);
}