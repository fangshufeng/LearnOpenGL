#version 330 core
out vec4 FragColor;

in vec3 Position;
in vec3 Normal;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
    float ratio = 1.00 / 1.52;
    vec3 dir = normalize(Position - cameraPos);
    vec3 r = refract(dir,normalize(Normal),ratio);
    
    FragColor = vec4(texture(skybox, r).rgb,1.0f);
}
