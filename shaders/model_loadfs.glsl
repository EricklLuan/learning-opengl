#version 140

struct Light {
	vec3 position;

	vec3 specular;
	vec3 diffuse;
	vec3 ambient;
};

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normals;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

uniform vec3 viewPos;
uniform Light light;

void main() {
	
	vec3 normal = normalize(Normals);
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(normal, lightDir), 0.0f);
	
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 lightRef = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, lightRef), 0.0f), 32.0f);

	vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;
	vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;
	vec3 specular = light.specular * spec * texture(texture_specular1, TexCoords).rgb;

	FragColor = vec4(ambient + diffuse + specular, 1.0f);
}