#version 140

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D diffuse_texture;
uniform samplerCube shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float far;

float shadowCalc(vec3 fragPos) {
	vec3 fragTolight = fragPos - lightPos;
	float closestDepth = texture(shadowMap, fragTolight).r;
	
	closestDepth *= far;

	float currentDepth = length(fragTolight);

	float bias = 0.05;

	float shadow = currentDepth - bias > closestDepth ? 1.0f : 0.0f;

	return shadow;
}

void main() {
	
	vec3 color = texture(diffuse_texture, TexCoords).rgb;
	vec3 lightColor = vec3(0.3f);

	vec3 ambient = 0.15f * lightColor;

	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfawayDir = normalize(viewDir + lightDir);

	float diff = max(dot(lightDir, normalize(Normal)), 0.0f);
	vec3 diffuse = diff * lightColor;

	float spec = pow(max(dot(normalize(Normal), halfawayDir), 0.0f), 64.0f);
	vec3 specular = spec * lightColor;

	float shadow = shadowCalc(FragPos);
	vec3 lighting = (ambient + (1.0f - 0.0f) * (diffuse + specular) ) * color;

	FragColor = vec4(lighting, 1.0f);
}