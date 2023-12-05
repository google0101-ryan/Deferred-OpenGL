#include "Shader.h"

#include <fstream>

Shader::Shader(std::string name)
{
    m_ProgramID = glCreateProgram();

    GLuint vert = MakeShader(GL_VERTEX_SHADER, "shaders/" + name + ".vert");
    GLuint frag = MakeShader(GL_FRAGMENT_SHADER, "shaders/" + name + ".frag");

    glAttachShader(m_ProgramID, vert);
    glAttachShader(m_ProgramID, frag);

    glLinkProgram(m_ProgramID);
    
    char infoLog[512];
    int status;
    glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        int len;
        glGetProgramInfoLog(m_ProgramID, 512, &len, infoLog);
        printf("%s\n", infoLog);
        exit(1);
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
}

void Shader::Bind()
{
    glUseProgram(m_ProgramID);
}

void Shader::SetUniformMat4x4(const char* name, glm::mat4 value)
{
    auto pos = glGetUniformLocation(m_ProgramID, name);
    assert(pos != -1);
    glUniformMatrix4fv(pos, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetUniformVec2(const char* name, glm::vec2 value)
{
    auto pos = glGetUniformLocation(m_ProgramID, name);
    assert(pos != -1);
    glUniform2f(pos, value.x, value.y);
}

void Shader::SetUniformVec2i(const char* name, glm::ivec2 value)
{
    auto pos = glGetUniformLocation(m_ProgramID, name);
    assert(pos != -1);
    glUniform2i(pos, value.x, value.y);
}

void Shader::SetUniformVec3(const char* name, glm::vec3 value)
{
    auto pos = glGetUniformLocation(m_ProgramID, name);
    assert(pos != -1);
    glUniform3f(pos, value.x, value.y, value.z);
}

void Shader::SetUniformInt(std::string name, int value)
{
    auto pos = glGetUniformLocation(m_ProgramID, name.c_str());
    assert(pos != -1);
    glUniform1i(pos, value);
}

GLuint Shader::MakeShader(GLenum type, std::string name)
{
    std::ifstream file(name, std::ios::ate | std::ios::binary);
    size_t size = file.tellg();
    
    char* buf = new char[size+1];
    file.seekg(0, std::ios::beg);
    file.read(buf, size);
    
    file.close();
    
    buf[size] = 0;

    int length = 0;
    GLuint ret = glCreateShader(type);
    glShaderSource(ret, 1, &buf, nullptr);
    glCompileShader(ret);

    char infoLog[512];
    int status;
    glGetShaderiv(ret, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        int len;
        glGetShaderInfoLog(ret, 512, &len, infoLog);
        printf("%s\n", infoLog);
        printf("%s\n", buf);
        exit(1);
    }

    delete[] buf;

    return ret;
}
