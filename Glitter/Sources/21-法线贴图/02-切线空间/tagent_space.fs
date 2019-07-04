
#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec3 Normal;
    mat3 TBN;
} fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform bool normalMapOpen;
uniform bool TBNReverse;

void main() {
    
    if (TBNReverse) { // 使用TNB的逆矩阵 操作键盘R键
        vec3 normal = normalize(fs_in.Normal);
        
        if (normalMapOpen) {
            // obtain normal from normal map in range [0,1]
            normal = texture(normalMap, fs_in.TexCoords).rgb;
            // transform normal vector to range [-1,1]
            normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
        }
        
        // get diffuse color
        vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
        // ambient
        vec3 ambient = 0.1 * color;
        // diffuse
        vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = diff * color;
        // specular
        vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
        
        vec3 specular = vec3(0.2) * spec;
        FragColor = vec4(ambient + diffuse + specular, 1.0);
    } else {

        vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
        
        //ambient
        vec3 ambient  = 0.1 * color;
        
        //diffuse
        vec3 lightDir = normalize(lightPos - fs_in.FragPos);
        
        vec3 norm = normalize(fs_in.Normal);
        
        if (normalMapOpen) { // 按键盘的空格键来控制是否开启发现贴图
            norm = texture(normalMap, fs_in.TexCoords).rgb;
            norm = normalize(norm * 2.0 - 1.0);
            norm = normalize(fs_in.TBN * norm);
        }
        
        float diff =  max(dot(norm,lightDir),0.0);
        vec3 diffuse = diff * color;
        
        //specular;
        vec3 reflectDir = reflect(-lightDir,norm);
        vec3 viewDir = normalize(viewPos - fs_in.FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(halfwayDir,norm),0.0),32.0f);
        vec3 specular =  vec3(0.2) * spec;
        
        FragColor = vec4(ambient + diffuse + specular, 1.0);
    }
    
}
