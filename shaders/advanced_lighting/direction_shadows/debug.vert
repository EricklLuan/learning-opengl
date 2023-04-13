#version 140

attribute vec3 aPosition;
attribute vec2 aTexCoords;

out vec2 TexCoords;

void main() {
	gl_Position = vec4(aPosition.xyz, 1.0f);
	TexCoords = aTexCoords;
}