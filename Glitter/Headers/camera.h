#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum class CameraMovement {
    UPWARD,
    DOWNWARD,
    LEFT,
    RIGHT
};

// a class stolen/inspired by https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h
class Camera {
private:
    glm::vec3 position;
    const glm::vec3 front{0.0f, 0.0f, -1.0f};
    const glm::vec3 up{0.0f, 1.0f, 0.0f};
    const glm::vec3 right{1.0f, 0.0f, 0.0f};

    // camera options
    float movement_speed;
    float mouse_sensitivity;
    float zoom;
public:
    Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), float speed_in = 2.5f, float sensitivity_in = 0.15f, float zoom_in = 1.0f)
        : position{ pos }, movement_speed{ speed_in }, mouse_sensitivity{ sensitivity_in }, zoom{ zoom_in } {}
    
    // Return the view matrix based on the current camera location
    glm::mat4 getViewMatrix() { return glm::lookAt(position, position + front, up); }
    
    // A callback for keyboard input to camera
    void processKeyboard(CameraMovement direction, float delta_time);
    // A callback for mouse input to camera
    void processMouseMovement(float xoffset, float yoffset);
    // A callback for mouse scrolling input. Simply subtracts the input from zoom.
    void processMouseScroll(float scroll_length) { zoom *= (scroll_length+mouse_sensitivity)/scroll_length; }

    // getters and setters
    float getZoom() { return zoom; }
    void setZoom(float in_zoom) { zoom = in_zoom; }
    float getMovementSpeed() { return movement_speed; }
    void setMovementSpeed(float in_speed) { movement_speed = in_speed; }
    float getMouseSensitivity() { return mouse_sensitivity; }
    void setMouseSensitivity(float in_sensitvity) { mouse_sensitivity = in_sensitvity; }
};
