#version 140

out vec4 fragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D boxTexture;
uniform sampler2D smileFaceTexture;

uniform float priority;

void main() {
  fragColor = mix(texture(boxTexture, texCoord), texture(smileFaceTexture, texCoord), priority) * vec4(ourColor, 1.0f);
}