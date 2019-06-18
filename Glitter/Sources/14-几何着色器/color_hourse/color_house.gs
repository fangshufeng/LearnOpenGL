#version 330 core

layout (points) in ;
layout (triangle_strip,max_vertices = 5) out ;

in VS_OUT {
    vec3 color;
} gs_in[];// 从顶点过来的总数数组的形式

out vec3 fColor;

void build_house(vec4 position) {
    
    gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4( 0.2, -0.2, 0.0, 0.0); // 2:bottom-right
    EmitVertex();
    gl_Position = position + vec4(-0.2,  0.2, 0.0, 0.0); // 3:top-left
    EmitVertex();
    gl_Position = position + vec4( 0.2,  0.2, 0.0, 0.0); // 4:top-right
    EmitVertex();
    gl_Position = position + vec4( 0.0,  0.4, 0.0, 0.0); // 5:top
    EmitVertex();
    
    EndPrimitive();
}

void main() {
    fColor = gs_in[0].color;
    build_house(gl_in[0].gl_Position);
}
