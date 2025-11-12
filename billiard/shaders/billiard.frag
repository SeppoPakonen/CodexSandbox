#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform int objectType; // 0: table, 1: ball, 2: cue
uniform vec3 ballColor;
uniform float materialShininess;

void main()
{
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result;
    if (objectType == 0) { // Table
        // Green table color with some texture variation
        vec3 tableColor = vec3(0.2, 0.3, 0.0);
        if (abs(FragPos.x) < 7.6 && abs(FragPos.z) < 13.65) { // Playing area
            tableColor = vec3(0.1, 0.5, 0.1); // Green playing area
        }
        result = (ambient + diffuse + specular) * tableColor;
    }
    else if (objectType == 1) { // Ball
        result = (ambient + diffuse + specular) * ballColor;
    }
    else if (objectType == 2) { // Cue
        // Wood-like color
        vec3 cueColor = vec3(0.8, 0.6, 0.2);
        result = (ambient + diffuse + specular) * cueColor;
    }
    else {
        result = (ambient + diffuse + specular) * vec3(1.0, 1.0, 1.0);
    }

    FragColor = vec4(result, 1.0);
}