#version 430 core

in vec3 WorldPos;
in vec3 Color;
in vec3 Normal;

layout(location = 0) out vec4 DiffuseOut;
layout(location = 1) out vec4 WorldPosOut;
layout(location = 2) out vec4 NormalOut;

void main()
{
    WorldPosOut = vec4(WorldPos, 1.0);
    DiffuseOut = vec4(Color, 1.0);
    NormalOut = vec4(Normal, 1.0);
}