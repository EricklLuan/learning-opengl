#version 140

attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec4 FragPosLightSpace;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 inverseModel;
uniform mat4 space;

void main() {
	FragPos = vec3(model*vec4(aPosition, 1.0f));
	Normal = transpose(mat3(inverseModel)) * aNormal;
	TexCoords = TexCoords;
	FragPosLightSpace = space * vec4(FragPos, 1.0f);
	gl_Position = projection * view * model * vec4(aPosition, 1.0f);
}