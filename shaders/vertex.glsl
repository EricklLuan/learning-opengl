#version 140

attribute vec3 position;
attribute vec3 color;
attribute vec2 textureCoord;

out vec3 ourColor;
out vec2 texCoord;

void main() {
  gl_Position = vec4(position, 1.0f);
  ourColor = color;
  texCoord = textureCoord;
}