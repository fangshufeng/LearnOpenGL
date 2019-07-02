
#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform bool normalMapOpen;

void main() {
    
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
    
    //ambient
    vec3 ambient  = 0.1 * color;
    
    //diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    
    vec3 norm = normalize(fs_in.Normal);
    
    if (normalMapOpen) { // 按键盘的空格键来控制是否开启发现贴图
        norm = texture(normalMap, fs_in.TexCoords).rgb;
        norm = normalize(norm * 2.0 - 1.0);
    }

    float diff =  max(dot(norm,lightDir),0.0);
    vec3 diffuse = diff * color;
    
    //specular;
    vec3 reflectDir = reflect(-lightDir,norm);
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),32.0f);
    vec3 specular =  vec3(0.2) * spec;
    
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
