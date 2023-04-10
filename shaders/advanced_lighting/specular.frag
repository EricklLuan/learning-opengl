#version 140

out vec4 FragColor;

in vec3 FragCoord;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D plank;

uniform vec3 lightPosition[4];
uniform vec3 lightColor[4];
uniform vec3 viewPosition;

vec3 blinn(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor) {
	
	vec3 lightDir = normalize(lightPos - fragPos);
	
	// diffuse:
	float dif = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = dif * lightColor;
	// **

	// specular:
	vec3 viewDir = normalize(viewPosition - fragPos);
	vec3 halfawayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfawayDir), 0.0f), 32.0f);
	vec3 specular = spec * lightColor;
	// **

	float dist = length(lightPos - fragPos);
	float attenuation = 1.0f / dist * dist;

	diffuse *= attenuation;
	specular *= attenuation;

	return diffuse + specular;
}

void main() {
	vec3 color = texture(plank, TexCoords).rgb;
	vec3 light = vec3(0.0f);

	for (int i=0; i<4; i++) {light += blinn(normalize(Normal), FragCoord, lightPosition[i], lightColor[i]);}

	color *= light;
	color = pow(color, vec3(1.0f/2.2f));

	FragColor = vec4(color, 1.0f);
}