#version 460 core

out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;

uniform vec3 lightPos;
uniform vec3 viewPos;

// in vec2 texCoord;
// uniform sampler2D ourTexture;

void main() 
{
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    // vec3 objColor = vec3(0.5, 0.1, 0.0);
    vec3 objColor = vec3(0.4, 0.35, 0.3);
    // FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);

    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.7;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objColor;
    FragColor = vec4(result, 1.0);
}
