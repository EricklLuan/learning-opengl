#version 140

attribute vec3 position;
attribute vec2 textureCoord;

out vec2 texCoord;

uniform mat4 transform;

void main() {
  gl_Position = transform * vec4(position, 1.0f);
  texCoord = textureCoord;
}