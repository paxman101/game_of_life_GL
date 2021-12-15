#include "camera.h"

void Camera::processKeyboard(CameraMovement direction, float delta_time) {
    float distance = movement_speed * delta_time;
    switch (direction) {
        case CameraMovement::UPWARD:
            position -= up * distance;
            break;

        case CameraMovement::DOWNWARD:
            position += up * distance;
            break;

        case CameraMovement::LEFT:
            position -= right * distance;
            break;

        case CameraMovement::RIGHT:
            position += right * distance;
            break;
    }
}

// TODO: Implement mouse movement for when cursor hits edge of screen like in
// top down games.
void Camera::processMouseMovement(float xoffset, float yoffset) {
    
}
