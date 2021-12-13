// Local Headers
#include "glitter.hpp"
#include "shader.h"
#include "util.h"
#include "grid.h"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

GLFWwindow *init_glfw_window() {
    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return nullptr;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    return mWindow;
}

void rendering_loop(GLFWwindow* window, GLuint shader_program) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    auto t_start = std::chrono::high_resolution_clock::now();

    glDrawArrays(GL_TRIANGLES, 0, 3);
    // Flip Buffers and Draw
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void run_loop(GLFWwindow* window, Shader shader_program) {
    GLint uniColor = glGetUniformLocation(shader_program.get_id(), "triangleColor");
    auto t_start = std::chrono::high_resolution_clock::now();

    while (glfwWindowShouldClose(window) == false) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    
        auto t_now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();

        glUniform3f(uniColor, (sin(time*4.0f)+1.0f)/2.0f, 0.0f, 0.0f);
            
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // Flip Buffers and Draw
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main(int argc, char * argv[]) {
    // init opengl
   // GLFWwindow *mWindow{ init_glfw_window() };

   // // vertex array object
   // GLuint vao;
   // glGenVertexArrays(1, &vao);
   // glBindVertexArray(vao);

   // // triangle shittery
   // std::vector<float> tri_vertices{0.0f, 0.5f,
   //                                 0.5f, -0.5f,
   //                                 -0.5, -0.5f};
   // 

   // // vertex buffer object
   // GLuint vbo;
   // glGenBuffers(1, &vbo);
   // glBindBuffer(GL_ARRAY_BUFFER, vbo);
   // glBufferData(GL_ARRAY_BUFFER, tri_vertices.size()*sizeof(float), &tri_vertices.front(), GL_STATIC_DRAW);


   // // load shaders
   // const char* vertex_source = readShader("./Shaders/test.vert");
   // const char* frag_source = readShader("./Shaders/test.frag");

   // // Make shader program from source filesj.
   // Shader main_shader{};
   // main_shader.compile(vertex_source, frag_source);
   // main_shader.use();

   // // 
   // glBindFragDataLocation(main_shader.get_id(), 0, "outColor");
   // glLinkProgram(main_shader.get_id());

   // // get position attribute
   // GLint pos_attrib = glGetAttribLocation(main_shader.get_id(), "position");
   // // Enable the attrib
   // glEnableVertexAttribArray(pos_attrib);
   // // Define attrib as 2 value float 
   // glVertexAttribPointer(pos_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

    Grid testGrid{ {{1,2},{1,3},{1,4}} };
    while (true) {
        testGrid.printState();
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        testGrid.update();
    }
   // // Rendering Loop
   // run_loop(mWindow, main_shader);

   // glfwTerminate();
    return EXIT_SUCCESS;
}
