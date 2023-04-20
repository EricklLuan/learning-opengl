#version 140

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure;

void main() {

	float gamma = 2.2f;
	vec3 color = texture(scene, TexCoords).rgb;
	vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
	color += bloomColor;
	vec3 mapped = vec3(1.0f) - exp(-color * exposure);
	mapped = pow(mapped, vec3(1.0f/gamma));
	FragColor = vec4(mapped, 1.0f);
	
}