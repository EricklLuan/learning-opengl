#version 140

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D frame;

void main() {
	float depthValue = texture(frame, TexCoords).r;
	FragColor = vec4(vec3(depthValue), 1.0f);
}