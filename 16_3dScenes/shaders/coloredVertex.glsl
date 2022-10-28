#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;

// send output data to the fragment shader
out vec4 fragColor;
out vec3 fragPosition;
out vec3 fragNormal;

// input uniforms
uniform mat4 uMvpMatrix;

void main()
{
    gl_Position = uMvpMatrix * vec4(aPos, 1.0);
    fragColor = vec4(aColor, 1.0);
    
    // TODO - update for lighting calculations
    fragPosition = aPos;
    fragNormal = aNormal;
}

