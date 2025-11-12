#version 330 core

// Vertex shader for modern rendering
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;
uniform vec3 waterDisp;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;
out float WaveHeight;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // Calculate wave height for water simulation
    WaveHeight = 0.3 * sin(FragPos.x * 0.2 + time) * cos(FragPos.z * 0.2 + time);
    vec3 modifiedPos = FragPos;
    modifiedPos.y += WaveHeight;
    
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;
    
    gl_Position = projection * view * vec4(modifiedPos, 1.0);
}