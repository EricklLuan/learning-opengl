#version 140

out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 camera_position;
uniform samplerCube skybox;

void main() {

	float ratio = 1.0f / 1.52f;
	vec3 I = normalize(Position - camera_position);
	vec3 R = refract(I, normalize(Normal), ratio);

	FragColor = vec4(texture(skybox, R).rgb, 1.0f);
}