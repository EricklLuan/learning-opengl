#version 140

struct Material {
	float shininess;
};

struct Light {
	vec3 position;

	vec3 diffuse;
	vec3 ambient;
	vec3 specular;
};

out vec4 fragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 LightPos;
in vec2 TexCoords;

uniform vec3 lightColor;
uniform vec3 viewPos;

uniform Material material;
uniform Light light;

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;
uniform sampler2D matrix;

void main() {
	
	vec3 t0 = texture(diffuseSampler, TexCoords).rgb;
	vec3 t1 = texture(specularSampler, TexCoords).rgb;
	vec3 t2 = vec3(0.0f);

	vec3 ambient = light.ambient * t0;
	
	// Diffuse
	vec3 norm = normalize(Normal);	
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * t0;
		
	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * t1;

	if (t1.rgb == vec3(0,0,0)) {
		t2 = texture(matrix, TexCoords).rgb;
	}

	vec3 result = (ambient + diffuse + specular + t2);
	fragColor = vec4(result, 1.0f);
}