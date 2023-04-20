#version 140

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D scene;
uniform float exposure;
uniform bool hdr;

void main() {

	float gamma = 2.2f;
	vec3 color = texture(scene, TexCoords).rgb;

	if (hdr) {
		vec3 mapped = vec3(1.0f) - exp(-color * exposure);
		mapped = pow(mapped, vec3(1.0f/gamma));
	
		FragColor = vec4(mapped, 1.0f);
	} else {
		FragColor = vec4(color, 1.0f);
	}
	
}