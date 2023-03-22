#version 140

struct Material {
	float shininess;
};

struct Light {
	vec3 position;
	vec3 direction;

	vec3 diffuse;
	vec3 ambient;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
	float cutOff;
	float outerCutOff;
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

void main() {
	
	vec3 t0 = texture(diffuseSampler, TexCoords).rgb;
	vec3 t1 = texture(specularSampler, TexCoords).rgb;

	vec3 ambient = light.ambient * t0;
	
	vec3 lightDir = normalize(light.position - FragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

	// Diffuse
	vec3 norm = normalize(Normal);	
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * diff * t0;
		
	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	vec3 specular = light.specular * spec * t1;

	float dist = length(light.position - FragPos);

	diffuse  *= intensity;
	specular *= intensity;
	
	vec3 result = (ambient + diffuse + specular);
	fragColor = vec4(result, 1.0f);
}