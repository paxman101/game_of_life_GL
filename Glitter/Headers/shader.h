#pragma once

#include<string>
#include<glad/glad.h>


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
};
