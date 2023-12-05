#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Framebuffer
{
public:
    void Bind();
    void Clear(GLbitfield clearTarget, glm::vec3 clearColor);
    void BlitTo(const Framebuffer& fbo, GLbitfield mask);
    void DefaultInit();

    int width, height;
    GLuint fbID;
    GLuint texColorBuffers[4], depthBuffer;
};

class DirShadowBuffer : public Framebuffer
{
public:
    bool SetupFramebuffer(int width, int height);
};