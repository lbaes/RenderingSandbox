#include "FreeCamera.hpp"
#include "glm/ext/matrix_transform.hpp"

namespace Eng {
    void FreeCamera::UpdateCameraPosByDirection(FreeCamera::Direction direction, float amount) {
        switch (direction) {
            case Direction::FRONT:
                _camera_pos += camera_front * amount;
                break;
            case Direction::BACK:
                _camera_pos -= camera_front * amount;
                break;
            case Direction::LEFT:
                _camera_pos -= glm::normalize(glm::cross(camera_front, _camera_up)) * amount;
                break;
            case Direction::RIGHT:
                _camera_pos += glm::normalize(glm::cross(camera_front, _camera_up)) * amount;
                break;
            case Direction::UP:
                _camera_pos += _camera_up * amount;
                break;
            case Direction::DOWN:
                _camera_pos -= _camera_up * amount;
                break;
        }
    }

    void FreeCamera::UpdateCameraView(float xAngle, float yAngle) {
        yaw += xAngle;
        pitch += yAngle;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        Vec3 direction{glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
                       glm::sin(glm::radians(pitch)),
                       glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))};
        camera_front = glm::normalize(direction);
        _view = glm::lookAt(_camera_pos, _camera_pos + camera_front, _camera_up);
    }
}