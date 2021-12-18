#include "app.h"
#include "util.h"
#include "shader.h"
#include "grid.h"
#include "renderer.h"
#include "patterns.h"
#include "camera.h"

#include <glm/glm.hpp>

#include <memory>
#include <chrono>
#include <thread>

void App::initGlfw() {
    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfw_window_ = glfwCreateWindow(app_configuration_.window_width, app_configuration_.window_height, 
                                    "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (glfw_window_ == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(glfw_window_);
    gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

    // Set window user pointer so we may define a wrapper function for this window's callbacks
    // https://stackoverflow.com/a/28660673
    glfwSetWindowUserPointer(glfw_window_, this);

    auto scrollCallbackWrapper = [](GLFWwindow *window, double x, double y) {
        static_cast<App*>(glfwGetWindowUserPointer(window))->scrollCallback(window, x, y);
    };

    auto cursorCallbackWrapper = [](GLFWwindow *window, double x, double y) {
        static_cast<App*>(glfwGetWindowUserPointer(window))->cursorCallback(window, x, y);
    };

    auto mouseButtonCallbackWrapper = [](GLFWwindow *window, int button, int action, int mods) {
        static_cast<App*>(glfwGetWindowUserPointer(window))->mouseButtonCallback(window, button, action, mods);
    };

    // Set callback functions
    glfwSetScrollCallback(glfw_window_, scrollCallbackWrapper);
    glfwSetCursorPosCallback(glfw_window_, cursorCallbackWrapper);
    glfwSetMouseButtonCallback(glfw_window_, mouseButtonCallbackWrapper);

}

void App::initRendering() {
    // load shaders
    const char* vertex_source = readShader("./Shaders/cell.vert");
    const char* frag_source = readShader("./Shaders/cell.frag");

    // Make shader program from source files.
    std::shared_ptr<Shader> main_shader = std::make_shared<Shader>();
    main_shader->compile(vertex_source, frag_source);
    main_shader->use();
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(app_configuration_.window_width), static_cast<float>( app_configuration_.window_height),
        0.0f, -1.0f, 1.0f) ;
    main_shader->setMatrix4("projection", projection);

    // renderer object
    cell_renderer_ = CellRenderer{main_shader};

    grid_renderer_ = GridRenderer{};
}

void App::init() {
    //init opengl
    initGlfw();
    initRendering();
}

void App::render() {
    processInput();
    std::this_thread::sleep_for(round<std::chrono::nanoseconds>(std::chrono::duration<double>{1./fps_}));

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    grid_renderer_.renderGrid(cell_grid_, cell_renderer_, update_grid_);

    glm::mat4 projection = glm::ortho(min_x, max_x, max_y, min_y, -1.0f, 1.0f);
    cell_renderer_.getShader()->setMatrix4("projection", projection);

    glm::mat4 view = camera_.getViewMatrix();
    cell_renderer_.getShader()->setMatrix4("view", view);
    
    glfwSwapBuffers(glfw_window_);
    glfwPollEvents();
}

void App::loadPatternFromFile(const std::string &file_path) {
    auto pattern_vec = ReadPatternFile(file_path);
    for (auto &pair : pattern_vec) {
        pair.first += 400;
        pair.second += 400;
    }
    cell_grid_ = Grid(pattern_vec);
}

void App::processInput() {
    if (glfwGetKey(glfw_window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(glfw_window_, true);

    // Camera movement checks
    if (glfwGetKey(glfw_window_, GLFW_KEY_W)) {
        camera_.processKeyboard(CameraMovement::UPWARD, 1);
    }
    if (glfwGetKey(glfw_window_, GLFW_KEY_S)) {
        camera_.processKeyboard(CameraMovement::DOWNWARD, 1);
    }
    if (glfwGetKey(glfw_window_, GLFW_KEY_D)) {
        camera_.processKeyboard(CameraMovement::RIGHT, 1);
    }
    if (glfwGetKey(glfw_window_, GLFW_KEY_A)) {
        camera_.processKeyboard(CameraMovement::LEFT, 1);
    }
    // Check for pause
    if (glfwGetKey(glfw_window_, GLFW_KEY_SPACE)) {
        update_grid_ = !update_grid_;
    }
    // Speed controls
    if (glfwGetKey(glfw_window_, GLFW_KEY_EQUAL)) {
        if (fps_ < 750) {
            fps_ += 5;
        }
    }
    if (glfwGetKey(glfw_window_, GLFW_KEY_MINUS)) {
        if (fps_ > 5) {
            fps_ -= 5;
        }
    }
}

void App::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    min_x += app_configuration_.window_width  * yoffset * zoom_factor_;
    max_x -= app_configuration_.window_width  * yoffset * zoom_factor_;
    min_y += app_configuration_.window_height * yoffset * zoom_factor_;
    max_y -= app_configuration_.window_height * yoffset * zoom_factor_;
}

void App::cursorCallback(GLFWwindow *window, double xoffset, double yoffset) {
    glm::mat4 view = camera_.getViewMatrix();
    float delta_x = max_x - min_y;
    float delta_y = max_y - min_y;
    float scaled_x = xoffset * delta_x / app_configuration_.window_width;
    float scaled_y = yoffset * delta_y / app_configuration_.window_height;

    glm::vec4 yep{scaled_x + 0.5*(app_configuration_.window_width - delta_x), 
                  scaled_y + 0.5*(app_configuration_.window_height - delta_y), 0.0f, 0.0f };
    yep.x -= view[3][0];
    yep.y -= view[3][1];
    mouse_last_x_ = yep.x;
    mouse_last_y_ = yep.y;
}

void App::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        //std::cout << "Clicked coord {" << mouse_last_x << "," << mouse_last_y << "}";
        std::pair<int, int> grid_coord = grid_renderer_.getGridCoord({ mouse_last_x_, mouse_last_y_ });
        std::cout << "Added cell at: " << grid_coord.first << ", " << grid_coord.second << std::endl;
        cell_grid_.addAlive(grid_coord);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        std::pair<int, int> grid_coord = grid_renderer_.getGridCoord({ mouse_last_x_, mouse_last_y_ });
        std::cout << "Killed cell at: " << grid_coord.first << ", " << grid_coord.second << std::endl;
        cell_grid_.killCell(grid_coord);
    }
}

App::App(const AppConfiguration &app_configuration): app_configuration_{app_configuration} {
    init();
}

App::~App() {
    glfwTerminate();
}

void App::run() {
    while (glfwWindowShouldClose(glfw_window_) == false) {
        render();
    }
}
