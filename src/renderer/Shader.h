#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

class Shader
{
public:
    Shader(std::string name);

    void Bind();
    void SetUniformMat4x4(const char* name, glm::mat4 value);
    void SetUniformVec2(const char* name, glm::vec2 value);
    void SetUniformVec2i(const char* name, glm::ivec2 value);
    void SetUniformVec3(const char* name, glm::vec3 value);
    void SetUniformInt(std::string name, int value);
private:
    GLuint MakeShader(GLenum type, std::string name);
private:
    GLuint m_ProgramID;
};

typedef std::unique_ptr<Shader> ShaderPtr;