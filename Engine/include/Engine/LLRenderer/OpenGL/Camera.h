#pragma once
#include "Engine/Core/Types/Mat.h"
#include "Engine/Core/Types/Vec.h"
#include <glm/gtx/transform.hpp>

namespace Eng {
    struct CameraPos {
        Vec3 camera_pos = Vec3{0.0f, 0.0f, 0.0f};
        Vec3 camera_front = Vec3{0.0f, 0.0f, -1.0f};
        Vec3 camera_up = Vec3{0.0f, 1.0f, 0.0f};
        float yaw = -90.0f, pitch = 0.0f;
    };

    class Camera {
    public:
        Camera() = default;
        Camera(Mat4 view, Mat4 projection, float fov, float aspect_ratio)
        : _view{view}, _projection{projection}, _fov(fov), _aspect_ratio(aspect_ratio) {}
        void SetCameraPos(CameraPos cameraPos){
            _pos = cameraPos;
            update_view_matrix();
        }

        void SetProjection(Mat4 projection) {
            _projection = projection;
        }

        void SetProjection(float fovDegrees, float aspectRatio) {
            update_projection_matrix(fovDegrees, aspectRatio);
        }

        Mat4 GetView() const {
            return _view;
        }

        Mat4 GetProjection() const{
            return _projection;
        }

    private:
        Mat4 _view{};
        Mat4 _projection{};
        CameraPos _pos{};
        float _fov{}, _aspect_ratio{};
        void update_view_matrix(){
            _view = glm::lookAt(_pos.camera_pos, _pos.camera_pos + _pos.camera_front, _pos.camera_up);
        }
        void update_projection_matrix(float fovDegrees, float aspectRatio){
            _fov = fovDegrees;
            _aspect_ratio = aspectRatio;
            _projection = glm::perspective(glm::radians(fovDegrees), aspectRatio, 0.1f, 100.0f);
        }
    };
}