#include "Engine/LLRenderer/Camera.h"
#include "glm/gtx/transform.hpp"

Eng::Camera::Camera() = default;

Eng::Camera::Camera(Eng::Vec3 cameraPos, Eng::Vec3 cameraTarget, Eng::Vec3 cameraUp, float fov, float aspectRatio,
		float zNear, float zFar)
		: _camera_pos{ cameraTarget }, _camera_target{ cameraTarget }, _camera_up{ cameraUp }
{
	_view = glm::lookAt(_camera_pos, _camera_target, _camera_up);
	_projection = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
}

void Eng::Camera::UpdateCamera(Eng::Vec3 cameraPos, Eng::Vec3 cameraTarget, Eng::Vec3 cameraUp)
{
	_camera_pos = cameraPos;
	_camera_target = cameraTarget;
	_camera_up = cameraUp;
	_view = glm::lookAt(_camera_pos, _camera_target, _camera_up);
}

void Eng::Camera::UpdateProjection(float fov, float aspectRatio, float zNear, float zFar)
{
	_projection = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
}

Eng::Mat4 Eng::Camera::GetView() const
{
	return _view;
}

Eng::Mat4 Eng::Camera::GetProjection() const
{
	return _projection;
}

Eng::Vec3 Eng::Camera::GetPosition() const
{
	return _camera_pos;
}

Eng::Vec3 Eng::Camera::GetTarget() const
{
	return _camera_target;
}

Eng::Vec3 Eng::Camera::GetUp() const
{
	return _camera_up;
}


