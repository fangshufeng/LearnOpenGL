
#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

struct PointLight{
    vec3 Position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    
    sampler2D diffuse;
    vec3 specular;
    
    float shiness;
};

uniform Material material;
uniform PointLight light;

uniform vec3 viewPos;
uniform vec3 lightPos;

void main() {
    
    // 环境光
    vec3 ambient = light.ambient * vec3(texture(material.diffuse,fs_in.TexCoords));
    
    // 漫反射光
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);
    float diff = max(dot(lightDir,normal),0.0);
    vec3 diffuse = diff * light.diffuse *  vec3(texture(material.diffuse,fs_in.TexCoords));
    
    // 镜面光
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec  = pow(max(dot(halfwayDir, normal), 0.0), material.shiness);
    vec3 specular = spec * light.specular * material.specular;
    
    FragColor = vec4(ambient + diffuse + specular,1.0);
}
