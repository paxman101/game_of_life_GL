// Local Headers
#include "glitter.hpp"
#include "shader.h"
#include "util.h"
#include "grid.h"
#include "renderer.h"

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
#include <memory>
#include <random>

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
}

void run_loop(GLFWwindow* window, CellRenderer &renderer) {
    auto t_start = std::chrono::high_resolution_clock::now();
    //GridRenderer grid_renderer{ {{1,2},{1,3},{1,4}} };
    GridRenderer grid_renderer{ {{1,1}, {1,2}, {2,1}, {3,1}, {5,1}, {5,3}, {5,4}, {5,5}, {4,3}, {3,4}, {2,4}, {3,5}, {1,5}} };
    while (glfwWindowShouldClose(window) == false) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
    
        std::this_thread::sleep_for(round<std::chrono::nanoseconds>(std::chrono::duration<double>{1./20}));

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //for (int y = 0; y < mHeight; y += 5) {
        //    for (int x = 0; x < mWidth; x += 5) {
        //        //renderer.DrawCell({ x,y }, { 4, 4, }, { 1.0,1.0,1.0 });
        //        //renderer.DrawCell({ x, y }, { 4, 4 }, { 0.5 * std::sin(y+time)*std::cos(x+time),  0.5 * std::cos(x + time), tan((x * y+ std::rand() + time) / 1000)});
        //    }
        //}
        grid_renderer.renderGrid(renderer);
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main(int argc, char * argv[]) {
    //init opengl
    GLFWwindow *mWindow{ init_glfw_window() };
     
    // load shaders
    const char* vertex_source = readShader("./Shaders/cell.vert");
    const char* frag_source = readShader("./Shaders/cell.frag");

    // Make shader program from source filesj.
    std::shared_ptr<Shader> main_shader = std::make_shared<Shader>();
    main_shader->compile(vertex_source, frag_source);
    main_shader->use();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(mWidth), static_cast<float>(mHeight),
        0.0f, -1.0f, 1.0f);
    main_shader->setMatrix4("projection", projection);

    // renderer object
    CellRenderer renderer{ main_shader };


    //Grid testGrid{ {{1,2},{1,3},{1,4}} };
    //while (true) {
    //    testGrid.printState();
    //    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    //    testGrid.update();
    //}
    // Rendering Loop
    run_loop(mWindow, renderer);

    glfwTerminate();
    return EXIT_SUCCESS;
}
