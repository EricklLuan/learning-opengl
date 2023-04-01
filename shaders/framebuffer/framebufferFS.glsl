#version 140

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture0;


void main() {
    const float offset = 1.0f / 500.0f;
	
	vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel0[9] = float[]( // blur
        1.0f / 16, 2.0f / 16, 1.0f / 16,
        2.0f / 16, 4.0f / 16, 2.0f / 16,
        1.0f / 16, 2.0f / 16, 1.0f / 16
    );

    float kernel1[9] = float[](
        -1.0f, -1.0f, -1.0f,
        -1.0f,  9.0f, -1.0f,
        -1.0f, -1.0f, -1.0f
    );

    float kernel2[9] = float[]( 
        1.0f, 1.0f, 1.0f,
        1.0f, -8.0f, 1.0f,
        1.0f, 1.0f, 1.0f
    );

    vec3 sampleTex[9];
    for (int i = 0;  i < 9; i++) {
        sampleTex[i] = vec3(texture(texture0, TexCoords.st + offsets[i]));
    }

    vec3 col = vec3(0.0f);
    for (int i = 0; i < 9; i++) {
        col += sampleTex[i] * kernel0[i];
    }
	
    FragColor = texture(texture0, TexCoords);
}