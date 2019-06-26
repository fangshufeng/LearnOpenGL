#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

//uniform sampler2D screenTexture;
uniform sampler2DMS screenTexture;

void main() {
//    vec3 col = texture(screenTexture, TexCoords).rgb;
//    FragColor = vec4(col, 1.0);
    ivec2 texSize = textureSize(screenTexture);

    vec4 fTexCol = vec4(0.0);
    for( int i = 0 ; i < 4 ; i++ ) {
        fTexCol += texelFetch(screenTexture,ivec2(TexCoords * texSize), i);
    }
    FragColor = fTexCol / 4;
}
