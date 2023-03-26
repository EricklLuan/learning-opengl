#version 140

attribute vec3 position;
attribute vec3 normals;
attribute vec2 texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 inverseModel;

out vec2 TexCoords;
out vec3 Normals;
out vec3 FragPos;

void main() {
	TexCoords = texCoords;
	Normals = mat3(transpose(inverseModel)) * normals;
	FragPos = vec3(model * vec4(position, 1.0f));
	gl_Position = projection * view * model * vec4(position, 1.0f);	
}