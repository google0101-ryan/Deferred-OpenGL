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
    vec4 worldPos = M * vec4(aPos, 1.0);
    WorldPos = worldPos.xyz;
    Color = aColor;

    mat3 normalMatrix = transpose(inverse(mat3(M)));
    Normal = normalMatrix * aNormal;

    gl_Position = VP * worldPos;
}