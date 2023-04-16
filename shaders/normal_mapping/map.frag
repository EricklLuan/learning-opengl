#version 140

out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

uniform sampler2D diffuse_texture;
uniform sampler2D normal_map;

void main() {
	vec3 color = texture(diffuse_texture, TexCoords).rgb;
	vec3 normal = texture(normal_map, TexCoords).rgb;
	vec3 lightColor = vec3(0.5f);

	normal = normal * 2.0f - 1.0f;
	normal = normalize(normal);

	vec3 ambient = 0.1f * color;

	vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
	vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
	vec3 halfawayDir = normalize(lightDir + viewDir);

	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * color;

	float spec = pow(max(dot(normal, halfawayDir), 0.0f), 32.0f);
	vec3 specular = vec3(0.3f) * spec;

	vec3 light = vec3(ambient + diffuse + specular);
	FragColor = vec4(light, 1.0f);
}