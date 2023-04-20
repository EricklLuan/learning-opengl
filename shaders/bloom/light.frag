#version 140

out vec4 FragColor;
out vec4 BrightColor;

uniform vec3 lightColor;

void main() {
	FragColor = vec4(lightColor, 1.0f);

	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 1.0f) {
		BrightColor = vec4(FragColor.rgb, 1.0f);
	} else {
		BrightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	}
}