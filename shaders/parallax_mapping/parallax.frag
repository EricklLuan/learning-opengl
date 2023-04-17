#version 140

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 TangentFragPos;
in vec3 TangentViewPos;
in vec3 TangentLightPos;

uniform sampler2D diffuse_texture;
uniform sampler2D normal_map;
uniform sampler2D displacement_map;

uniform float height_scale;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir) {
	const float minLayers =  8;
	const float maxLayers = 32;
	float layers = mix(maxLayers, minLayers, abs(dot(vec3(0.0f, 0.0f, 1.0f), viewDir)));

	float layersDepth = 1.0f / layers;
	float currentLayerDepth = 0.0;

	vec2 p = viewDir.xy * height_scale;
	vec2 deltaTexCoords = p/layers;	

	vec2 currentTexCoords = texCoords;
	float currentDepthMapValue = texture(displacement_map, currentTexCoords).r;

	while (currentLayerDepth < currentDepthMapValue) {
		currentTexCoords = currentTexCoords - deltaTexCoords;
		currentDepthMapValue = texture(displacement_map, currentTexCoords).r;
		currentLayerDepth += layersDepth;
	}

	vec2 previusTexCoords = currentTexCoords + deltaTexCoords;

	float afterDepth = currentDepthMapValue - currentLayerDepth;
	float beforeDepth = texture(displacement_map, previusTexCoords).r - currentLayerDepth + layersDepth;

	float weight = afterDepth / (afterDepth - beforeDepth);
	vec2 finalCoords = previusTexCoords * weight + currentTexCoords * (1.0f - weight);

	return finalCoords;
}

void main() {
	vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
	vec2 texCoords = TexCoords;
	
	texCoords = ParallaxMapping(TexCoords, viewDir);

	if (texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0) {
		discard;
	}

	vec3 color = texture(diffuse_texture, texCoords).rgb;
	vec3 normal = texture(normal_map, texCoords).rgb;

	normal = normalize(normal * 2.0f - 1.0f);

	vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
	vec3 halfawayDir = normalize(lightDir + viewDir);
	
	vec3 ambient = 0.1f * color;

	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * color;

	float spec = pow(max(dot(normal, halfawayDir), 0.0f), 32.0f);
	vec3 specular = vec3(0.2f) * spec;

	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}