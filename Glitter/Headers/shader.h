#pragma once

#include<string>
#include<glad/glad.h>
#include<glm/gtc/type_ptr.hpp>


// Shader type for handling basic shader programs.
// inspired/partially stolen from https://learnopengl.com/code_viewer_gh.php?code=src/7.in_practice/3.2d_game/0.full_source/shader.cpp
class Shader {
private:
    GLuint id;
    enum ObjectType {
        VERTEX,
        FRAGMENT,
        GEOMETRY,
        PROGRAM,
    };
    void checkCompileErrors(GLuint object, ObjectType type);
public:
    Shader() = default;
    // Compile shader program given 
    void compile(const char* vertex_source, const char* fragment_source);
    // Set this shader as active 
    Shader& use() { glUseProgram(id); return *this; }
    GLuint get_id() { return id; }

    void setBool(const std::string & name, bool value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
    }

    void setInt(const std::string & name, int value) const {
        glUniform1i(glGetUniformLocation(id, name.c_str()), value);
    }

    void setFloat(const std::string & name, float value) const {
        glUniform1f(glGetUniformLocation(id, name.c_str()), value);
    }

    void setDouble(const std::string & name, double value) const {
        glUniform1d(glGetUniformLocation(id, name.c_str()), value);
    }

    void setVector2f(const std::string& name, const glm::vec2& value) const {
        glUniform2f(glGetUniformLocation(id, name.c_str()), value.x, value.y);
    }

    void setVector3f(const std::string& name, const glm::vec3& value) const {
        glUniform3f(glGetUniformLocation(id, name.c_str()), value.x, value.y, value.z);
    }

    void setMatrix2(const std::string& name, const glm::mat2& matrix) {
        glUniformMatrix2fv(glGetUniformLocation(id, name.c_str()), 1, false, glm::value_ptr(matrix));
    }

    void setMatrix4(const std::string& name, const glm::mat4& matrix) {
        glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, false, glm::value_ptr(matrix));
    }
};
