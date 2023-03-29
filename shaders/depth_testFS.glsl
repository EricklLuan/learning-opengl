#version 140

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture0;

float near = 0.1f;
float far = 100.0f;

float linearizeDepth(float depth) {
	float z = depth * 2.0f - 1.0f;
	return (2.0f * near * far) / (far + near - z * (far - near));
}

void main() {
	float depth = linearizeDepth(gl_FragCoord.z) / far;
	FragColor = vec4(vec3(depth), 1.0f);
}