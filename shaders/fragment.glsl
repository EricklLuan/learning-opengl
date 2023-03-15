#version 140

out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D boxTexture;
uniform sampler2D smileFaceTexture;

void main() {
  fragColor = mix(texture(boxTexture, texCoord), texture(smileFaceTexture, texCoord), texture(smileFaceTexture, texCoord).a * 0.2);
}