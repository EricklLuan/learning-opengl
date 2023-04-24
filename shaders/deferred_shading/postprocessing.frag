#version 140

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light {
	vec3 position;
	vec3 color;

	float linear;
	float quadratic;
	float radius;
};

const int NR_LIGHTS = 8;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

void main() {

	vec3 FragPos = texture(gPosition, TexCoords).rgb;
	vec3 Normal = texture(gNormal, TexCoords).rgb;
	vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
	
	float Specular = texture(gAlbedoSpec, TexCoords).a;
	
	vec3 lighting = Diffuse * 0.1f;
	vec3 viewDir = normalize(viewPos - FragPos);

	for (int i=0; i < NR_LIGHTS; ++i) {
		float dist = length(lights[i].position - FragPos);

		if (dist < lights[i].radius) {
			vec3 lightDir = normalize(lights[i].position - FragPos);
			vec3 diffuse = max(dot(Normal, lightDir),0.0f) * Diffuse * lights[i].color;

			vec3 halfawayDir = normalize(lightDir + viewDir);
			float spec = pow(max(dot(Normal, halfawayDir),0.0f), 16.0f);
			vec3 specular = lights[i].color * spec * Specular;

			float attenuation = 1.0f / (1.0f + lights[i].linear * dist + lights[i].quadratic * dist * dist);
		
			diffuse *= attenuation;
			specular *= attenuation;

			lighting += diffuse + specular;
		}
	}

	FragColor = vec4(lighting, 1.0f);
}