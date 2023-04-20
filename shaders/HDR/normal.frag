#version 140

out vec4 FragColor;

struct Light {
	vec3 position;
	vec3 color;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D diffuse_texture;
uniform Light lights[16];

void main() {
	
	vec3 color = texture(diffuse_texture, TexCoords).rgb;
	vec3 normal = normalize(Normal);

	vec3 light = vec3(0.0f);
	vec3 ambient = 0.0 * color;

	for (int i=0; i < 16; i++){
		vec3 lightDir = normalize(lights[i].position - FragPos);
		float diff = max(dot(lightDir, normal),0.0f);
		vec3 diffuse = lights[i].color * diff * color;
		vec3 result = diffuse;

		float dist = length(FragPos - lights[i].position);
		result *= 1.0f / (dist * dist);
		light += result;
	}

	FragColor = vec4(ambient + light, 1.0f);

}