#pragma once

#include "Shader.h"
#include <vector>

class Obj
{
private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    GLuint vao, vbo, ibo;
public:
    void Load(std::string& filename);
};