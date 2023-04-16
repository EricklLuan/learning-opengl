#version 140

attribute vec3 aPosition;
attribute vec3 aNormal;
attribute vec2 aTexCoords;
attribute vec3 aTangent;
attribute vec3 aBitangent;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 inverseModel;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
	FragPos = vec3(model * vec4(aPosition, 1.0f));
	TexCoords = aTexCoords;

	mat3 normal = transpose(mat3(inverseModel));
	vec3 T = normalize(normal * aTangent);
	vec3 N = normalize(normal * aNormal);
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N,T);

	mat3 TBN = transpose(mat3(T, B, N));

	TangentFragPos = TBN * FragPos;
	TangentLightPos = TBN * lightPos;
	TangentViewPos = TBN * viewPos;

	gl_Position = projection * view * model * vec4(aPosition, 1.0f);
}