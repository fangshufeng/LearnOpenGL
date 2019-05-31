


#version 330 core

out vec4 FragColor;

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;

in vec3 Normal;
in vec3 FragPos;

void main()
{
    // 1.环境光照
    float ambientStrength = 0.1;
    vec3 ambient  = ambientStrength * lightColor;
    
    // 2.漫反射
    vec3 lightDirection = normalize(lightPos - FragPos);
    vec3 norm = normalize(Normal);
    float diff =  max(dot(norm,lightDirection),0.0);
    vec3 diffuse = diff * lightColor;

    FragColor = vec4((ambient + diffuse) * objectColor,1.0);
}
