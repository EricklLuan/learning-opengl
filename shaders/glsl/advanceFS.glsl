#version 140

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D metal;
uniform sampler2D container;

void main() {
	
	if (gl_FrontFacing)
		FragColor = texture(metal, TexCoords);
	else 
		FragColor = texture(container, TexCoords);

}