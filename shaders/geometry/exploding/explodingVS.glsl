#version 140

attribute vec3 position;
attribute vec3 normals;
attribute vec2 texCoords;

out vec2 TextureCoords;
out vec3 Normal;

layout (std140) uniform Matrices {
	mat4 projection;
	mat4 view;
};

uniform mat4 model;

void main() {
	TextureCoords = texCoords;
	gl_Position = projection * view * model * vec4(position, 1.0f);
}