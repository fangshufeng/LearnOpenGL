
#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 lightSpacePos;
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

uniform sampler2D shadowMap;

float ShadowCalculation(vec4 fragPosLightSpace) {
    
    // 执行透视除法
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // 变换到[0,1]的范围
    projCoords = projCoords * 0.5 + 0.5;
    // 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // 取得当前片元在光源视角下的深度
    float currentDepth = projCoords.z;
    // 检查当前片元是否在阴影中
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
    return shadow;
}

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

    float shadow = ShadowCalculation(fs_in.lightSpacePos);

    vec3 lighting = (ambient + diffuse + specular) - (shadow * (diffuse + specular));
    
    FragColor = vec4(lighting,1.0);
}
