#include "renderManager.h"
#include <GLFW/glfw3.h>

float ScreenQuad[] = 
{
    -1.0f, 1.0f, 0.0f,     0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f,    0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,      1.0f, 1.0f,
    
    -1.0f, -1.0f, 0.0f,    0.0f, 0.0f,
    1.0f, 1.0f, 0.0f,      1.0f, 1.0f,
    1.0f, -1.0f, 0.0f,     1.0f, 0.0f
};

struct Light
{
    glm::vec3 pos;
    glm::vec3 color;
};

Light pointLights[1] =
{
    {{0.0f, 0.0f, 0.0f}, {0.5f, 0.5f, 0.5f}},
};

GLuint screenVAO, screenVBO;

bool RenderManager::startUp()
{
    initFBOs();

    glEnable(GL_FRAMEBUFFER_SRGB); 

    srand(13);

    plane.Move(glm::vec3(0.0f, -1.0f, 0.0f));
    plane.Scale(glm::vec3(10.0f, 0.0f, 10.0f));

    dirShadowShader = std::make_unique<Shader>("shadowShader");
    depthShader = std::make_unique<Shader>("depthPass");
    colorShader = std::make_unique<Shader>("colorPass");
    pointLightShader = std::make_unique<Shader>("pointLightPass");
    dirLightShader = std::make_unique<Shader>("dirShadowPass");

    glGenVertexArrays(1, &screenVAO);
    glGenBuffers(1, &screenVBO);

    glBindVertexArray(screenVAO);
    glBindBuffer(GL_ARRAY_BUFFER, screenVBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(ScreenQuad), ScreenQuad, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindVertexArray(0);

    pointLightShader->Bind();
    pointLightShader->SetUniformInt("colorBuffer", 0);
    pointLightShader->SetUniformInt("positionBuffer", 1);
    pointLightShader->SetUniformInt("normalBuffer", 2);
    pointLightShader->SetUniformInt("shadowMap", 3);
    dirLightShader->Bind();
    dirLightShader->SetUniformInt("colorBuffer", 0);
    dirLightShader->SetUniformInt("positionBuffer", 1);
    dirLightShader->SetUniformInt("shadowMap", 2);


    for (int i = 0; i < sizeof(pointLights) / sizeof(Light); i++)
    {
        float x = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
        float y = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 4.0);
        float z = static_cast<float>(((rand() % 100) / 100.0) * 6.0 - 3.0);
        pointLights[i].pos = glm::vec3(x, y, z);
        float rColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
        float gColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
        float bColor = static_cast<float>(((rand() % 100) / 200.0f) + 0.5); // between 0.5 and 1.0
        pointLights[i].color = glm::vec3(rColor, gColor, bColor);
    }

    return true;
}

glm::vec3 playerPos = glm::vec3(-2.0f, 10.0f, 6.0f);

void RenderManager::Render()
{
    glEnable(GL_DEPTH_TEST);
    glDepthMask(true);
    glDepthFunc(GL_LESS);
    glDisable(GL_CULL_FACE);

    // Directional light shadow map pass

    glm::vec3 cubePositions[] = {
        glm::vec3(-3.0, -0.5, -3.0),
        glm::vec3(0.0, -0.5, -3.0),
        glm::vec3(3.0, -0.5f, -3.0),
        glm::vec3(-3.0, -0.5, 0.0),
        glm::vec3(0.0, -0.5, 0.0),
        glm::vec3(3.0, -0.5f, 0.0),
        glm::vec3(-3.0, -0.5, 3.0),
        glm::vec3(0.0, -0.5, 3.0),
        glm::vec3(3.0, -0.5f, 3.0),
    };

    glm::mat4 shadowProj = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 10.0f);
    glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
                                    glm::vec3(0.0f, 0.0f, 0.0f),
                                    glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 lightSpaceMatrix = shadowProj * lightView;
    
    if (shadowMapDirty)
    {
        dirShadowFBO.Bind();
        dirShadowFBO.Clear(GL_DEPTH_BUFFER_BIT, glm::vec3(1.0f));
        dirShadowShader->Bind();

        dirShadowShader->SetUniformMat4x4("VP", lightSpaceMatrix);

        for (int i = 0; i < 9; i++)
        {
            cube.ResetPosition();
            cube.Move(cubePositions[i]);
            cube.Scale(glm::vec3(0.25));
            cube.Draw(dirShadowShader);
        }
        shadowMapDirty = false;

        plane.Draw(dirShadowShader);
    }
    
    // Depth-only pass
    renderFBO.Bind();
    renderFBO.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, glm::vec3(0.0f));
    glm::mat4 p = glm::perspective(glm::radians(45.0f), 1024.f / 768.f, 1.0f, 1000.0f);
    glm::mat4 v = glm::lookAt(playerPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    glm::mat4 vp = p * v;
    depthShader->Bind();
    depthShader->SetUniformMat4x4("VP", vp);

    for (int i = 0; i < 9; i++)
    {
        cube.ResetPosition();
        cube.Move(cubePositions[i]);
        cube.Scale(glm::vec3(0.25));
        cube.Draw(depthShader);
    }

    plane.Draw(depthShader);

    // G-Buffer pass

    glDepthMask(GL_FALSE);
    glDepthFunc(GL_EQUAL);

    // We render all objects in the scene into a normal buffer, position buffer, diffuse buffer
    colorShader->Bind();
    colorShader->SetUniformMat4x4("VP", vp);
    for (int i = 0; i < 9; i++)
    {
        cube.ResetPosition();
        cube.Move(cubePositions[i]);
        cube.Scale(glm::vec3(0.25));
        cube.Draw(colorShader);
    }

    plane.Draw(colorShader);

    // Lighting pass, generates realistic lighting
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);

    pointLightShader->Bind();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);

    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, renderFBO.texColorBuffers[0]);
    // glActiveTexture(GL_TEXTURE1);
    // glBindTexture(GL_TEXTURE_2D, renderFBO.texColorBuffers[1]);
    // glActiveTexture(GL_TEXTURE2);
    // glBindTexture(GL_TEXTURE_2D, dirShadowFBO.depthBuffer);

    // dirLightShader->Bind();
    // dirLightShader->SetUniformMat4x4("lightSpaceMatrix", lightSpaceMatrix);
    
    // glBindVertexArray(screenVAO);
    // glDrawArrays(GL_TRIANGLES, 0, 6);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderFBO.texColorBuffers[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, renderFBO.texColorBuffers[1]);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, renderFBO.texColorBuffers[2]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, dirShadowFBO.depthBuffer);

    pointLightShader->SetUniformVec3("viewPos", playerPos);
    
    for (int i = 0; i < sizeof(pointLights) / sizeof(Light); i++)
    {
        pointLightShader->Bind();
        pointLightShader->SetUniformMat4x4("lightSpaceMatrix", lightSpaceMatrix);
        pointLightShader->SetUniformVec3("light.Position", pointLights[i].pos);
        pointLightShader->SetUniformVec3("light.Color", pointLights[i].color);
        // Draw the buffer to the screen
        glBindVertexArray(screenVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glDisable(GL_BLEND);
}

bool RenderManager::initFBOs()
{
    dirShadowFBO.SetupFramebuffer(4096, 4096);

    renderFBO.width = 1024;
    renderFBO.height = 768;
    renderFBO.DefaultInit();

    return true;
}
