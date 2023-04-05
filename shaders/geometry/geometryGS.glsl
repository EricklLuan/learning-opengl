#version 140
#extension GL_ARB_geometry_shader4:enable

layout (points) in;
layout (triangle_strip, max_vertices=5) out;

in vec3 Color[];

out vec3 fColor;

void draw_house(vec4 position) {
	fColor = Color[0];
	gl_Position = position + vec4(-0.2f, -0.2f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = position + vec4(0.2f, -0.2f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = position + vec4(-0.2f, 0.2f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = position + vec4(0.2f, 0.2f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = position + vec4(0.0f, 0.4f, 0.0f, 0.0f);
	fColor = vec3(1.0f, 1.0f, 1.0f);
	EmitVertex();
	EndPrimitive();
}

void main() {
	draw_house(gl_in[0].gl_Position);
}