#version 140

attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexCoords;

out vec3 Normal;
out vec3 FragCoord;
out vec2 TexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform mat4 inverseModel;

void main() {
	gl_Position = projection * view * model * vec4(aPosition, 1.0f);
	Normal = mat3(transpose(inverseModel)) * aNormal;
	FragCoord = vec3(model * vec4(aPosition, 1.0f));
	TexCoords = aTexCoords;
}