#version 140

attribute vec3 position;
attribute vec3 normal;

out vec3 Normal;
out vec3 Position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 inverse_model;

void main() {
	Normal = mat3(transpose(inverse_model)) * normal;
	Position = vec3(model * vec4(position, 1.0f));
	gl_Position = projection * view * model * vec4(position, 1.0f);
}