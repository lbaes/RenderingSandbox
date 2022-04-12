#pragma once
#include "Engine/Core/Types/Mat.h"
#include "Engine/Core/Types/Vec.h"

namespace Eng {
    class Camera {
    public:
        Camera();
		Camera(Vec3 cameraPos, Vec3 cameraTarget, Vec3 cameraUp, float fov, float aspectRatio, float zNear = 0.1f, float zFar= 5000.0f);
		void UpdateCamera(Vec3 cameraPos, Vec3 cameraTarget, Vec3 cameraUp);
		void UpdateProjection(float fov, float aspectRatio, float zNear = 0.1f, float zFar= 5000.0f);
		Mat4 GetView() const;
		Mat4 GetProjection() const;
		Vec3 GetPosition() const;
		Vec3 GetTarget() const;
		Vec3 GetUp() const;
	protected:
		Vec3 _camera_pos{};
		Vec3 _camera_target{};
		Vec3 _camera_up{};
		Mat4 _view{};
		Mat4 _projection{};
	};
}