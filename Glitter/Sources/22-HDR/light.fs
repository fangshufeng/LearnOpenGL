
#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

struct Light {
    vec3 Position;
    vec3 Color;
};

uniform Light lights[16];

uniform sampler2D diffuseMap;

void main() {
    
    vec3 color = texture(diffuseMap, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    
    // ambient
    vec3 ambient = 0.0 * color;
    
    // lighting
    vec3 lighting = vec3(0.0);
    for(int i = 0; i < 16; i++) {
        // diffuse
        vec3 lightDir = normalize(lights[i].Position - fs_in.FragPos);
        float diff = max(dot(lightDir, -normal), 0.0);
        vec3 diffuse = lights[i].Color * diff * color;
        vec3 result = diffuse;
        
        float distance = length(fs_in.FragPos - lights[i].Position);
        float attenuation = 1 / (1.0 + 0.09 * distance +
                                 0.032 * (distance * distance));
        result *= attenuation;
        
        lighting += result;
    }

    FragColor = vec4(lighting, 1.0);
}
