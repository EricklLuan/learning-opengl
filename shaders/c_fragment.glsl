#version 140

struct Material {
	float shininess;
};

struct DirectionalLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
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
uniform PointLight pointLight[4];
uniform DirectionalLight dirLight;

uniform sampler2D diffuseSampler;
uniform sampler2D specularSampler;

vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
	
	vec3 t0 = texture(diffuseSampler, TexCoords).rgb;
	vec3 t1 = texture(specularSampler, TexCoords).rgb;

	vec3 ambient = light.ambient * t0;
	
	vec3 lightDir = normalize(light.position - FragPos);

	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = smoothstep(0.0f, 1.0f, (theta - light.outerCutOff) / epsilon);

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
	float atenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	vec3 result = calcDirLight(dirLight, Normal, viewDir);
	
	for (int i = 0; i < 4; i++) {
		result += calcPointLight(pointLight[i], Normal, FragPos, viewDir);
	}

	result += (ambient * intensity * atenuation) + (diffuse * intensity * atenuation) + (specular * intensity * atenuation);

	fragColor = vec4(result, 1.0f);
}

vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(lightDir, normalize(normal)), 0.0f);
	
	vec3 reflectDir = reflect(-lightDir, normalize(normal));
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

	vec3 ambient = light.ambient * texture(diffuseSampler, TexCoords).rgb;
	vec3 diffuse = light.diffuse * diff * texture(diffuseSampler, TexCoords).rgb;
	vec3 specular = light.specular * spec * texture(specularSampler, TexCoords).rgb;

	return (ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 lightDir = normalize(light.position - FragPos);
	float diff = max(dot(lightDir, normalize(normal)), 0.0f);

	vec3 reflectDir = reflect(-lightDir, normalize(normal));
	float spec = pow(max(dot(reflectDir, viewDir),0.0f), material.shininess);

	float dist = length(light.position - fragPos);
	float atenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	vec3 ambient = (light.ambient * texture(diffuseSampler, TexCoords).rgb) * atenuation;
	vec3 diffuse = (light.diffuse * diff * texture(diffuseSampler, TexCoords).rgb) * atenuation;
	vec3 specular = (light.specular * spec * texture(specularSampler, TexCoords).rgb) * atenuation;

	return (ambient + diffuse + specular);
}