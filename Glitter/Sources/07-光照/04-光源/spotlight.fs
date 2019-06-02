
#version 330 core

out vec4 FragColor;

uniform vec3 cameraPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

struct SpotLight {
    vec3 position;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant; // 衰减函数的常数项
    float linear; // 衰减函数的一次项
    float quadratic; // 衰减函数的二次项
    
    vec3 spotDir;
    float cutOff;
};

struct Material {
    
    sampler2D diffuse;
    sampler2D specular;
    
    float shininess;
};

uniform Material material;
uniform SpotLight light;

void main() {
    
   vec3 lightDirection = normalize(light.position - FragPos);
     // 计算Thetaθ角的余弦值
    float theta = dot(lightDirection, normalize(-light.spotDir));
    
    if (theta < light.cutOff) {// 比较的余弦值的大小，余弦值越大角度越小，
        FragColor = vec4 (light.ambient * vec3(texture(material.diffuse,TexCoords)) ,1.0f);
    } else {
       // 1.环境光照
        vec3 ambient  = light.ambient * vec3(texture(material.diffuse,TexCoords));

        // 2.漫反射
        vec3 norm = normalize(Normal);
        float diff =  max(dot(norm,lightDirection),0.0);
        vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse,TexCoords));

        // 3.镜面光
        vec3 reflectDir = reflect(-lightDirection,norm);
        vec3 viewDir = normalize(cameraPos - FragPos);
        float spec = pow(max(dot(viewDir,reflectDir),0.0), material.shininess);
        vec3 specular =   spec * light.specular * vec3(texture(material.specular,TexCoords));

        // 衰减值
        float distance = length(light.position - FragPos);
        float attenuation = 1 / (light.constant + light.linear * distance +
                                 light.quadratic * (distance * distance));

        vec3 result = ambient + diffuse + specular;

        result  *= attenuation;
      FragColor = vec4(result  ,1.0);
    }

}
