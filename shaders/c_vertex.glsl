#version 140

attribute vec3 position;
attribute vec3 normal;
attribute vec2 textureCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 inverseModel;

out vec3 Normal;
out vec3 FragPos;
out vec3 LightPos;
out vec2 TexCoords;

void main() {
	FragPos = vec3(model * vec4(position, 1.0f));
	Normal = mat3(transpose(inverseModel)) * normal;
	TexCoords = textureCoords;

	gl_Position = projection * view * vec4(FragPos, 1.0f);
}