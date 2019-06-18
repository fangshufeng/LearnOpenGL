#version 330 core
out vec4 FragColor;

in vec3 Position;
in vec3 Normal;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    vec3 dir = normalize(Position - cameraPos);
    vec3 r = reflect(dir,normalize(Normal));
    
    FragColor = vec4(texture(skybox, r).rgb,1.0f);
//    FragColor = vec4(1.0f);
}
