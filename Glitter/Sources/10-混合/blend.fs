
#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;

void main() {
    
   vec4 textu =  texture(texture1, TexCoord);
    
    if (textu.a < 0.5) {
        discard;
    }
    
    FragColor = vec4(texture(texture1, TexCoord).rgb,1.0f);
}
