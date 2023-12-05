#pragma once

#include "framebuffer.h"
#include "Shader.h"
#include "Cube.h"
#include "Plane.h"

class RenderManager
{
public:
    bool startUp();

    void Render();
private:
    bool initFBOs();
private:
    DirShadowBuffer dirShadowFBO;
    Framebuffer renderFBO;
    ShaderPtr dirShadowShader, depthShader, colorShader, pointLightShader, dirLightShader;
    Cube cube;
    Plane plane;
};