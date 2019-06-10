
// 线性关系请打开下面的注释
#version 330 core

in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;

void main() {
//    FragColor = vec4(texture(texture1, TexCoord).rgb,1.0f);
    FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}

//线性关系
//#version 330 core
//out vec4 FragColor;
//
//float near = 0.1;
//float far  = 100.0;
//
//float LinearizeDepth(float depth)
//{
//    float z = depth * 2.0 - 1.0; // back to NDC
//    return (2.0 * near * far) / (far + near - z * (far - near));
//}
//
//void main()
//{
//    float depth = LinearizeDepth(gl_FragCoord.z) / far; // 为了演示除以 far
//    FragColor = vec4(vec3(depth), 1.0);
//}
