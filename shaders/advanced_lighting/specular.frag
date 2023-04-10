#version 140

out vec4 FragColor;

in vec3 FragCoord;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D plank;

uniform vec3 lightPosition;
uniform vec3 viewPosition;
uniform bool blinn;

void main() {
	
	vec3 lightDir = normalize(lightPosition - FragCoord);
	vec3 viewDir = normalize(viewPosition - FragCoord);
	
	float dif = max(dot(lightDir, normalize(Normal)), 0.0f);
	vec3 diffuse = dif * texture(plank, TexCoords).rgb;

	vec3 ambient = 0.05f * texture(plank, TexCoords).rgb;

	float spec;
	
	if (blinn) {
		vec3 halfawayDir = normalize(lightDir + viewDir);
		spec = pow(max(dot(normalize(Normal), halfawayDir), 0.0f), 32.0f);
	} else {
		vec3 reflectDir = reflect(-lightDir, normalize(Normal));
		spec = pow(max(dot(viewDir, reflectDir), 0.0f), 8.0f);
	}

	vec3 specular = vec3(0.3f) * spec;
	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}