
#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;


out vec3 Color;

uniform vec2 offsets[100];

void main() {
    Color = aColor;
    gl_Position = vec4(aPos + offsets[gl_InstanceID],0.0,1.0);
}
