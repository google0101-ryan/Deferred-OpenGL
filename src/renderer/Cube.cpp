#include "Cube.h"

#include <glm/gtc/matrix_transform.hpp>

GLfloat cube_vertices[] = 
{
    // Position              // Color             // Normal
    -1.0f, -1.0f, -1.0f,     0.0f, 0.0f, 1.0f,    0.0f, 0.0f, -1.0f,
    1.0f, -1.0f, -1.0f,      0.0f, 1.0f, 0.0f,    0.0f, 0.0f, -1.0f,
    1.0f, 1.0f, -1.0f,       1.0f, 0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
    1.0f, 1.0f, -1.0f,       0.0f, 0.0f, 1.0f,    0.0f, 0.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,      0.0f, 1.0f, 0.0f,    0.0f, 0.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,     1.0f, 0.0f, 0.0f,    0.0f, 0.0f, -1.0f,
    
    -1.0f, -1.0f, 1.0f,      0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f,       0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,        1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,        0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,
    -1.0f, 1.0f, 1.0f,       0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,      1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
    
    -1.0f, 1.0f, 1.0f,       0.0f, 0.0f, 1.0f,    -1.0f, 0.0f, 0.0f, 
    -1.0f, 1.0f, -1.0f,      0.0f, 1.0f, 0.0f,    -1.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,     1.0f, 0.0f, 0.0f,    -1.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,     0.0f, 0.0f, 1.0f,    -1.0f, 0.0f, 0.0f,
    -1.0f, -1.0f, 1.0f,      0.0f, 1.0f, 0.0f,    -1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,       1.0f, 0.0f, 0.0f,    -1.0f, 0.0f, 0.0f,
    
    1.0f, 1.0f, 1.0f,        0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, -1.0f,       0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 1.0f,
    1.0f, -1.0f, -1.0f,      1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
    1.0f, -1.0f, -1.0f,      0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,
    1.0f, -1.0f, 1.0f,       0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f,        1.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,
    
    -1.0f, -1.0f, -1.0f,     0.0f, 0.0f, 1.0f,    0.0f, -1.0f, 0.0f,
    1.0f, -1.0f, -1.0f,      0.0f, 1.0f, 0.0f,    0.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 1.0f,       1.0f, 0.0f, 0.0f,    0.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 1.0f,       0.0f, 0.0f, 1.0f,    0.0f, -1.0f, 0.0f,
    -1.0f, -1.0f, 1.0f,      0.0f, 1.0f, 0.0f,    0.0f, -1.0f, 0.0f,
    -1.0f, -1.0f, -1.0f,     1.0f, 0.0f, 0.0f,    0.0f, -1.0f, 0.0f,
    
    -1.0f, 1.0f, -1.0f,     0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, -1.0f,      0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f,       1.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 1.0f,       0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 1.0f,      0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, -1.0f,     1.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,
};

Cube::Cube()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*9, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*9, (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float)*9, (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Cube::Move(glm::vec3 dir)
{
    model = glm::translate(model, dir);
}

void Cube::Scale(glm::vec3 dir)
{
    model = glm::scale(model, dir);
}

void Cube::Draw(ShaderPtr& shader)
{
    shader->SetUniformMat4x4("M", model);

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
