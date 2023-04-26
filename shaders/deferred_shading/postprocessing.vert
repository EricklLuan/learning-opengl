#version 140

in vec3 aPosition;
in vec2 aTexCoords;

out vec2 TexCoords;

void main() {
	TexCoords = aTexCoords;
	gl_Position = vec4(aPosition, 1.0f);
}