#version 140

attribute vec3 position;
attribute vec3 color;

out vec3 ourColor;

void main() {
  gl_Position = vec4(position, 1.0f);
  ourColor = color;
}