#version 430 core

in vec3 WorldPos;
in vec3 Color;
in vec3 Normal;

layout(location = 0) out vec3 DiffuseOut;
layout(location = 1) out vec3 WorldPosOut;
layout(location = 2) out vec3 NormalOut;

void main()
{
    WorldPosOut = WorldPos;
    DiffuseOut = Color;
    NormalOut = Normal;
}