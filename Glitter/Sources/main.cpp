// Local Headers
#include "glitter.hpp"
#include "shader.h"
#include "util.h"
#include "grid.h"
#include "renderer.h"
#include "camera.h"

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

float fps{20.0f};
const float cell_size{ 20.0f };
float zoom{ 1.0f };
// a bool to keep track of whether or not to update the GOL grid.
bool update_grid{ false };

// last mouse values from mouse movement callback
float mouse_last_x{};
float mouse_last_y{};

// Make camera object with movement speed of cell_size.
Camera camera{ {0.0, 0.0, 0.0}, cell_size};

GridRenderer grid_renderer{ {{1,1}, {1,2}, {2,1}, {3,1}, {5,1}, {5,3}, {5,4}, {5,5}, {4,3}, {3,4}, {2,4}, {3,5}, {1,5}} };

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

//void rendering_loop(GLFWwindow* window, GLuint shader_program) {
//}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Camera movement checks
    if (glfwGetKey(window, GLFW_KEY_W)) {
        camera.processKeyboard(CameraMovement::UPWARD, 1);
    }
    if (glfwGetKey(window, GLFW_KEY_S)) {
        camera.processKeyboard(CameraMovement::DOWNWARD, 1);
    }
    if (glfwGetKey(window, GLFW_KEY_D)) {
        camera.processKeyboard(CameraMovement::RIGHT, 1);
    }
    if (glfwGetKey(window, GLFW_KEY_A)) {
        camera.processKeyboard(CameraMovement::LEFT, 1);
    }
    // Check for pause
    if (glfwGetKey(window, GLFW_KEY_SPACE)) {
        update_grid = !update_grid;
    }
    // Speed controls
    if (glfwGetKey(window, GLFW_KEY_EQUAL)) {
        if (fps < 500) {
            fps += 5;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_MINUS)) {
        if (fps > 5) {
            fps -= 5;
        }
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    camera.processMouseScroll(yoffset);
}

// cursor_callback to find cursor location relative to world space
void cursor_callback(GLFWwindow* window, double xoffset, double yoffset) {
    glm::mat4 view = camera.getViewMatrix();
    glm::vec4 yep{ xoffset/zoom, yoffset/zoom, 0.0f, 0.0f };
    yep.x -= view[3][0];
    yep.y -= view[3][1];
    mouse_last_x = yep.x;
    mouse_last_y = yep.y;
}

// mouse button callback for capturing click
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    // if button is left click
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        //std::cout << "Clicked coord {" << mouse_last_x << "," << mouse_last_y << "}";
        std::pair<int, int> grid_coord = grid_renderer.getGridCoord({ mouse_last_x, mouse_last_y });
        std::cout << "Added cell at: " << grid_coord.first << ", " << grid_coord.second << std::endl;
        grid_renderer.getGrid().addAlive(grid_coord);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        std::pair<int, int> grid_coord = grid_renderer.getGridCoord({ mouse_last_x, mouse_last_y });
        std::cout << "Killed cell at: " << grid_coord.first << ", " << grid_coord.second << std::endl;
        grid_renderer.getGrid().killCell(grid_coord);
    }
}

void run_loop(GLFWwindow* window, CellRenderer &renderer) {
    auto t_start = std::chrono::high_resolution_clock::now();
    //GridRenderer grid_renderer{ {{1,2},{1,3},{1,4}} };
    while (glfwWindowShouldClose(window) == false) {
        processInput(window);
        std::this_thread::sleep_for(round<std::chrono::nanoseconds>(std::chrono::duration<double>{1./fps}));

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        grid_renderer.renderGrid(renderer, update_grid);

        zoom = camera.getZoom();
        glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(mWidth) / zoom, static_cast<float>(mHeight) / zoom, 0.0f, -1.0f, 1.0f);
        renderer.getShader()->setMatrix4("projection", projection);

        glm::mat4 view = camera.getViewMatrix();
        renderer.getShader()->setMatrix4("view", view);
    
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

int main(int argc, char * argv[]) {
    //init opengl
    GLFWwindow *mWindow{ init_glfw_window() };

    // set callback
    glfwSetScrollCallback(mWindow, scroll_callback);
    glfwSetCursorPosCallback(mWindow, cursor_callback);
    glfwSetMouseButtonCallback(mWindow, mouse_button_callback);
     
    // load shaders
    const char* vertex_source = readShader("./Shaders/cell.vert");
    const char* frag_source = readShader("./Shaders/cell.frag");

    // Make shader program from source filesj.
    std::shared_ptr<Shader> main_shader = std::make_shared<Shader>();
    main_shader->compile(vertex_source, frag_source);
    main_shader->use();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(mWidth), static_cast<float>(mHeight),
        0.0f, -1.0f, 1.0f) ;
    main_shader->setMatrix4("projection", projection);

    // renderer object
    CellRenderer cell_renderer{main_shader};

    // Rendering Loop
    run_loop(mWindow, cell_renderer);

    glfwTerminate();
    return EXIT_SUCCESS;
}
