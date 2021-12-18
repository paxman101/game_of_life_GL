#pragma once

#include "grid.h"
#include "renderer.h"
#include "camera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

// Class App handles all high-level operations of running the application
class App {
public:
    struct AppConfiguration {
        int window_width;
        int window_height;
        float initial_fps;
        float initial_cell_size;
        float inital_zoom_factor;
    };
private:
    AppConfiguration app_configuration_;
    GLFWwindow *glfw_window_;
    Grid cell_grid_;
    CellRenderer cell_renderer_;
    GridRenderer grid_renderer_;
    float fps_ = app_configuration_.initial_fps;
    float cell_size_ = app_configuration_.initial_cell_size;
    float zoom_factor_ = app_configuration_.inital_zoom_factor;

    Camera camera_{{app_configuration_.window_width* cell_size_ / 2.f, app_configuration_.window_height * cell_size_ / 2.f, 0.0f}, 
        cell_size_, zoom_factor_};

    bool update_grid_ = false;
    
    float mouse_last_x_;
    float mouse_last_y_;

    float max_x = app_configuration_.window_width;
    float min_x = 0;
    float max_y = app_configuration_.window_height;
    float min_y = 0;

    // Initializes GLFW window and callbacks.
    void initGlfw();
    // Initializes everything needed for rendering the grid
    void initRendering();
    // Initializes everything needed to for run() to be called
    void init();
    
    // Render function that is called during rendering loop
    void render();


public:
    App(const AppConfiguration& app_configuration);
    ~App();

    // Loads a pattern from the given filename and sets the grid to the pattern
    void loadPatternFromFile(const std::string &file_path);

    // A function that polls GLFW for keyboard inputs. Will be refactored into a
    // proper callback function.
    void processInput();
    // Callback for GLFW to call when there is an event of mouse scrolling.
    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    // Callback for GLFW to call when the mouse cursor moves
    void cursorCallback(GLFWwindow* window, double xoffset, double yoffset);
    // Callback for GLFW to call when a mouse button is pressed
    void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);

    // Runs app until a termination condition is met.
    void run();
};
