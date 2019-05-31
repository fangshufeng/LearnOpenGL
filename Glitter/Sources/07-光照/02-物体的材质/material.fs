


#version 330 core

out vec4 FragColor;

uniform vec3 cameraPos;

in vec3 Normal;
in vec3 FragPos;

struct Light {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float shininess;
};

uniform Material material;
uniform Light light;

void main() {
    // 1.环境光照
    vec3 ambient  = light.ambient * material.ambient;
    
    // 2.漫反射
    vec3 lightDirection = normalize(light.position - FragPos);
    vec3 norm = normalize(Normal);
    float diff =  max(dot(norm,lightDirection),0.0);
    vec3 diffuse = diff * light.diffuse * material.diffuse;

    // 3.镜面光
    vec3 reflectDir = reflect(-lightDirection,norm);
    vec3 viewDir = normalize(cameraPos - FragPos);
    float spec = pow(max(dot(viewDir,reflectDir),0.0), material.shininess);
    vec3 specular =   spec * light.specular * material.specular;
    
    FragColor = vec4((ambient + diffuse + specular),1.0);
}
