#version 430 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec3 aNormal;

uniform mat4 M;
uniform mat4 VP;

out vec3 WorldPos;
out vec3 Color;
out vec3 Normal;

void main()
{
    gl_Position = VP * M * vec4(aPos, 1.0);
    Color = aColor;
    WorldPos = vec3(M * vec4(aPos, 1.0));
    mat3 normMatrix = transpose(inverse(mat3(M)));
    Normal = normMatrix * aNormal;
}