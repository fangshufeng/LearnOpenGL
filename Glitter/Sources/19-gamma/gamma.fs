
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

uniform bool gamma;

void main() {

//    // 环境光
//    vec3 ambient = light.ambient * vec3(texture(material.diffuse,fs_in.TexCoords));
//
//    // 漫反射光
//    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
//    vec3 normal = normalize(fs_in.Normal);
//    float diff = max(dot(lightDir,normal),0.0);
//    vec3 diffuse = diff * light.diffuse *  vec3(texture(material.diffuse,fs_in.TexCoords));
//
//    // 镜面光
//    vec3 reflectDir = reflect(-lightDir, normal);
//    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shiness);
//    vec3 specular = spec * light.specular * material.specular;
    
//    FragColor = vec4(ambient + diffuse + specular,1.0);
    
    //    rgb(178,34,34) #B222222   11674146
    //    rgb(185,33,40) #B92128    12132648
    
//    vec3 color = vec3(178.0 / 255.0f,34.0 / 255.0f,34.0 / 255.0f);
//    vec3 color = vec3(185.0 / 255.0f,33.0 / 255.0f,40.0 / 255.0f);
    
//       vec3 color = vec3(0.5,0.0f,0.0f);
//    color = pow(color,vec3(2.2f));
//    FragColor = vec4(color,1.0f);
    
    // rgb(185,33,40) #B92128    12132648  0.725
    //  不校正 #7c0405 rgb(124,4,5)         0.486
    //  gamma校正  b7262c rgb(183,38,44)
    vec3 color = texture(material.diffuse,fs_in.TexCoords).rgb;
//    if (gamma) {
//         color = pow(color,vec3(1/2.2f));
//    }
    FragColor = vec4(color,1.0f);
}
