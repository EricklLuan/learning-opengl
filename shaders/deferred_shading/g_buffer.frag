#version 140

out vec3 gPosition;
out vec3 gNormal;
out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main() {
	gPosition = FragPos;
	gNormal = normalize(Normal);
	gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
	gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;
}