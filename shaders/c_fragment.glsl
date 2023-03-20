#version 140

out vec4 fragColor;

uniform vec3 cubeColor;
uniform vec3 lightColor;

void main() {
  fragColor = vec4(cubeColor * lightColor, 1.0f);
}