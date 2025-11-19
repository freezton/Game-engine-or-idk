#version 460 core

out vec4 FragColor;

in vec3 fragPos;

uniform vec3 viewPos;

void main() 
{
    float dist = length(fragPos - viewPos);
    float alpha = clamp(1.0 - dist / 50, 0.0, 1.0);

    FragColor = vec4(vec3(0.8), alpha);
}
