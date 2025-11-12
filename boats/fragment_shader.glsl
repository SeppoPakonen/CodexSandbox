#version 330 core

// Fragment shader for modern rendering
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in float WaveHeight;

uniform float time;
uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
    // Simple lighting
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, Normal);
    
    float diff = max(dot(Normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    
    vec3 ambient = 0.1 * lightColor;
    vec3 diffuse = diff * lightColor;
    vec3 specular = spec * lightColor;
    
    // Water-like blue color with wave effect
    vec3 objectColor = vec3(0.1, 0.2, 0.4);  // Base water color
    
    // Apply wave effect to color
    float waveEffect = abs(WaveHeight) * 0.5;
    objectColor += vec3(waveEffect * 0.1, waveEffect * 0.2, waveEffect * 0.3);
    
    vec3 result = ambient + (diffuse + specular) * objectColor;
    
    FragColor = vec4(result, 1.0);
}