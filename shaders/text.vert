#version 140

in vec4 a_Vertex;

out vec2 TexCoords;

uniform mat4 projection;

void main() {

	gl_Position = projection * vec4(a_Vertex.xy, 0.0, 1.0);
	TexCoords = a_Vertex.zw;
}
