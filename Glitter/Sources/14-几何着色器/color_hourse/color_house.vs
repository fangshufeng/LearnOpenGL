
#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;


out VS_OUT {
    vec3 color;
} vs_out;

void main() {
    vs_out.color = aColor;
    gl_Position  = vec4(aPos,0.0,1.0);
}


/**
 *
 ERROR::SHADER_COMPILATION_ERROR of type: GEOMETRY
 ERROR: 0:6: inputs in the geometry shader must be arrays
 ERROR: 0:25: Use of undeclared identifier 'fColor'
 
 -- --------------------------------------------------- --
 ERROR::PROGRAM_LINKING_ERROR of type: PROGRAM
 ERROR: One or more attached shaders not successfully compiled
 
 -- --------------------------------------------------- --
 Program ended with exit code: 0
*/
