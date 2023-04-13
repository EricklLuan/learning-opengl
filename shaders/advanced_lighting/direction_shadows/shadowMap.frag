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

float shadowCalc(vec4 fragPos, vec3 normal, vec3 lightDir) {

	vec3 projCoords = fragPos.xyz / fragPos.w;
	projCoords = projCoords * 0.5 + 0.5;

	if (projCoords.z > 1.0f) {
		return 0.0f;
	}

	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005f);
	float shadow = 0.0f;
	vec2 texelSize = 1.0f / textureSize(shadowMap, 0);
	for (int x = -1; x <= 1; ++x) {
		for (int y = -1; y <= 1; ++y) {
			float pcf = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcf ? 1.0f : 0.0f;
		}
	}

	shadow /= 9.0f;

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

	float shadow = shadowCalc(FragPosLightSpace, normalize(Normal), lightDir);
	vec3 lighting = (ambient + (1.0f - shadow) * (diffuse + specular)) * color;

	FragColor = vec4(lighting, 1.0f);
}