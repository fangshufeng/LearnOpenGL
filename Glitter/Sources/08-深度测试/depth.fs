
#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;

void main() {
    FragColor = vec4(texture(texture1, TexCoord).rgb,1.0f);
}
