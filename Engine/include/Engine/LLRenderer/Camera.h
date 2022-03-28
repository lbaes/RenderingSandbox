#pragma once
#include "Engine/Core/Types/Mat.h"
#include "Engine/Core/Types/Vec.h"

namespace Eng {
    struct CameraPos {
        Vec3 camera_pos = Vec3{0.0f, 0.0f, 0.0f};
        Vec3 camera_target = Vec3{ 0.0f, 0.0f, -1.0f};
        Vec3 camera_up = Vec3{0.0f, 1.0f, 0.0f};
        float yaw = -90.0f, pitch = 0.0f;
    };

    class Camera {
    public:
        Camera() = default;
        Camera(Mat4 view, Mat4 projection, float fov, float aspect_ratio);
        void SetCameraPos(CameraPos cameraPos);
        void SetProjection(Mat4 projection);
        void SetProjection(float fovDegrees, float aspectRatio);
        Mat4 GetView() const;
        Mat4 GetProjection() const;
		Vec3 GetPosition() const;

	private:
        Mat4 _view{};
        Mat4 _projection{};
        CameraPos _pos{};
        float _fov{}, _aspect_ratio{};
        void update_view_matrix();
        void update_projection_matrix(float fovDegrees, float aspectRatio);
	};
}