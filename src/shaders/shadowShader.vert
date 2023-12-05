#version 430 core

layout(location = 0) in vec3 aPos;

uniform mat4 M;
uniform mat4 VP;

void main()
{
    gl_Position = VP * M * vec4(aPos, 1.0);
}