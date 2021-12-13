#include <iostream>

#include "shader.h"



void Shader::compile(const char* vertex_source, const char* fragment_source)
{
    unsigned int sVertex, sFragment;
    // vertex Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertex_source, NULL);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, VERTEX);
    // fragment Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragment_source, NULL);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, FRAGMENT);
    // if geometry shader source code is given, also compile geometry shader
    // shader program
    this->id = glCreateProgram();
    glAttachShader(this->id, sVertex);
    glAttachShader(this->id, sFragment);
    glLinkProgram(this->id);
    checkCompileErrors(this->id, PROGRAM);
    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
}

void Shader::checkCompileErrors(GLuint object, ObjectType type)
{
    int success;
    char infoLog[1024];
    if (type != PROGRAM)
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}
