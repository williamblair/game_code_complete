#version 330 core

out vec4 FragColor;

// inputs from vertex shader
in vec4 fragColor;
in vec3 fragPosition;
in vec3 fragNormal;

void main()
{
    FragColor = fragColor;
}

