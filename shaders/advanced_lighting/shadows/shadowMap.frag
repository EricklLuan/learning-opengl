#version 140

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 viewPos;
uniform vec3 lightPos;

float shadowCalc(vec4 fragPos) {
	vec3 projCoords = fragPos.xyz / fragPos.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float shadow = currentDepth > closestDepth ? 1.0f : 0.0f;

	return shadow;
}

void main() {
	
	vec3 color = texture(diffuseTexture, TexCoords).rgb;
	vec3 lightDir = normalize(lightPos - FragPos);
	vec3 lightColor = vec3(0.3);
	
	vec3 ambient = 0.15f * lightColor;

	float diff = max(dot(lightDir, normalize(Normal)), 0.0f);
	vec3 diffuse = diff * lightColor;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 halfawayDir = normalize(viewDir + lightDir);
	float spec = pow(max(dot(halfawayDir, normalize(Normal)), 0.0f), 64.0f);
	vec3 specular = spec * lightColor;

	float shadow = shadowCalc(FragPosLightSpace);
	vec3 lighting = (ambient + (1.0f - shadow) * (diffuse + specular)) * color;

	FragColor = vec4(lighting, 1.0f);
}