#version 140

attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexCoords;
attribute vec3 aTangent;
attribute vec3 aBitangent;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 TangentFragPos;
out vec3 TangentViewPos;
out vec3 TangentLightPos;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 inverseModel;

void main() {
	FragPos = vec3(model * vec4(aPosition, 1.0f));
	TexCoords = aTexCoords;
	
	// TBN
	vec3 T = normalize(mat3(model) * aTangent);
	vec3 B = normalize(mat3(model) * aBitangent);
	vec3 N = normalize(mat3(model) * aNormal);
	mat3 TBN = mat3(T,B,N);

	TangentFragPos = TBN * FragPos;
	TangentViewPos = TBN * viewPos;
	TangentLightPos = TBN * lightPos;

	gl_Position = projection * view * model * vec4(aPosition, 1.0f);
}