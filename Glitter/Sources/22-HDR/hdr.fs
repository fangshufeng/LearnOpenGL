#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform bool hdr;
uniform float exposure;

void main()
{
//    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
//    hdrColor = pow(hdrColor, vec3(1.0 / 2.2));
//    FragColor = vec4(hdrColor, 1.0);
    
//    const float gamma = 2.2;
//    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
//
//    // Reinhard色调映射
//    vec3 mapped = hdrColor / (hdrColor + vec3(1.0));
//    // Gamma校正
//    mapped = pow(mapped, vec3(1.0 / gamma));
//
//    FragColor = vec4(mapped, 1.0);
    

    const float gamma = 2.2;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    
    // 曝光色调映射
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    // Gamma校正
    mapped = pow(mapped, vec3(1.0 / gamma));
    
    FragColor = vec4(mapped, 1.0);
    
}
