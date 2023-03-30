#version 140

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture0;

void main() {
	vec4 textureColor = texture(texture0, TexCoords);

	FragColor = textureColor;
}