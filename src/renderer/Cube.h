#pragma once

#include "Shader.h"

class Cube
{
private:
    GLuint vao, vbo, ibo;

    glm::mat4 model = glm::mat4(1.0f);
public:
    Cube();

    void ResetPosition() {model = glm::mat4(1.0f);}
    void Move(glm::vec3 dir);
    void Scale(glm::vec3 scale);

    void Draw(ShaderPtr& shader);
};