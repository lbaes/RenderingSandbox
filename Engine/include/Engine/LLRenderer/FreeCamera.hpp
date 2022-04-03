#pragma once
#include "Camera.h"

namespace Eng {
    class FreeCamera : public Camera {
    public:
        enum class Direction {
            FRONT,
            BACK,
            LEFT,
            RIGHT,
            UP,
            DOWN
        };
        void UpdateCameraPosByDirection(Direction direction, float amount);
        void UpdateCameraView(float xAngle, float yAngle);
    private:
        Vec3 camera_front;
        float yaw = 0.0f;
        float pitch = 0.0f;
    };
}