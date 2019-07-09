
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform sampler2D bloomBlur;

uniform bool openToneMapping;
uniform bool openExposure;
uniform bool openBloom;

uniform float exposure;

void main()
{
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
    
    if (openBloom) {
        hdrColor += bloomColor;
    }
    
    if (openToneMapping) { // 开启色调映射
        //  曝光(Exposure) 算法
        /*
         * 通过 键盘E键 开启该算法
         * 通过键盘的 ⬆️ ⬇️ 键调整曝光度 查看效果
         */
        if (openExposure) {
           hdrColor = vec3(1.0) - exp(-hdrColor * exposure);
        } else {    // 默认的算法是Reinhard色调映射
             hdrColor = hdrColor / (hdrColor + vec3(1.0));
        }
    
    }
    FragColor = vec4(hdrColor, 1.0);
}
