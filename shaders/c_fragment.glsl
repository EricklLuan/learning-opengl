#version 140

out vec4 fragColor;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 cubeColor;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main() {

	vec3 norm = normalize(Normal);	
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColor;
		
	float ambientStrength = 0.1f;
	vec3 ambient = ambientStrength * lightColor;

	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
	vec3 specular = specularStrength * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * cubeColor;
	fragColor = vec4(result, 1.0f);
}