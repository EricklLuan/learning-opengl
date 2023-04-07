#version 140

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D colorBuffer;

void main() {
	vec3 col = texture(colorBuffer, TexCoords).rgb;
    float grayscale = 0.2126 * col.r + 0.7152 * col.g + 0.0722 * col.b;
    FragColor = vec4(vec3(grayscale), 1.0);
}