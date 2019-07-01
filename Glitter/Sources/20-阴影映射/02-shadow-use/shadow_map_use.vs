
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// 可以将点通过这个矩阵转换到光空间  注意这个矩阵不包括model  所以在s乘它之前先进行model变换
uniform mat4 lightSpaceMatrix;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 lightSpacePos;
} vs_out;

void main() {
    vs_out.FragPos = vec3(model * vec4(aPos,1.0f));
    vs_out.Normal = aNormal;
    vs_out.TexCoords = aTexCoords;
    vs_out.lightSpacePos = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos,1.0f);
}

