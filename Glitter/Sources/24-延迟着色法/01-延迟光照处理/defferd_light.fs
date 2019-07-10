
#version 330 core

out vec4 FragColor;

uniform sampler2D positionMap;
uniform sampler2D normalMap;
uniform sampler2D diffuseMap;

in vec2 TexCoords;

struct Light {
    vec3 Position;
    vec3 Color;
    
    float Linear;
    float Quadratic;
};
const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

void main() {
   
    vec3 FragPos = texture(positionMap,TexCoords).rgb;
    vec3 Normal = texture(normalMap,TexCoords).rgb;
    vec3 Diffuse = texture(diffuseMap,TexCoords).rgb;
    float Specular = texture(diffuseMap,TexCoords).a;
    
    vec3 lighting  = Diffuse * 0.1; // hard-coded ambient component
    vec3 viewDir  = normalize(viewPos - FragPos);
    for(int i = 0; i < NR_LIGHTS; ++i)  {
        // Diffuse
        vec3 lightDir = normalize(lights[i].Position - FragPos);
        vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lights[i].Color;
        // Specular
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
        vec3 specular = lights[i].Color * spec * Specular;
        // Attenuation
        float distance = length(lights[i].Position - FragPos);
        float attenuation = 1.0 / (1.0 + lights[i].Linear * distance + lights[i].Quadratic * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;
        lighting += diffuse + specular;
    }
    
    FragColor = vec4(lighting,1.0f);
}
