#version 140
#extension GL_ARB_geometry_shader4:enable

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

in vec2 TextureCoords[];
out vec2 TexCoords;

uniform float time;

vec3 getNormal() {
	vec3 a = gl_in[0].gl_Position.xyz - gl_in[1].gl_Position.xyz;
	vec3 b = gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz;
	return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal) {
	float magnitude = 3.0f;
	vec3 direction = normal * ((sin(time) + 1.0f) / 2.0f) * magnitude;
	return position + vec4(direction, 0.0f); 
}

void main() {
	vec3 Normal = getNormal();

	gl_Position = explode(gl_in[0].gl_Position, Normal);
	TexCoords = TextureCoords[0];
	EmitVertex();

	gl_Position = explode(gl_in[1].gl_Position, Normal);
	TexCoords = TextureCoords[1];
	EmitVertex();

	gl_Position = explode(gl_in[2].gl_Position, Normal);
	TexCoords = TextureCoords[2];
	EmitVertex();

	EndPrimitive();
}