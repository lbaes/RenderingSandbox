#include "Engine/LLRenderer/Camera.h"
#include "glm/gtx/transform.hpp"

void Eng::Camera::SetProjection(Eng::Mat4 projection)
{
	_projection = projection;
}

Eng::Camera::Camera(Eng::Mat4 view, Eng::Mat4 projection, float fov, float aspect_ratio)
		: _view{view}, _projection{projection}, _fov(fov), _aspect_ratio(aspect_ratio) {}

void Eng::Camera::SetCameraPos(Eng::CameraPos cameraPos)
{
	_pos = cameraPos;
	update_view_matrix();
}

void Eng::Camera::SetProjection(float fovDegrees, float aspectRatio)
{
	update_projection_matrix(fovDegrees, aspectRatio);
}

Eng::Mat4 Eng::Camera::GetView() const
{
	return _view;
}

Eng::Mat4 Eng::Camera::GetProjection() const
{
	return _projection;
}

void Eng::Camera::update_view_matrix()
{
	_view = glm::lookAt(_pos.camera_pos, _pos.camera_target, _pos.camera_up);
}

void Eng::Camera::update_projection_matrix(float fovDegrees, float aspectRatio)
{
	_fov = fovDegrees;
	_aspect_ratio = aspectRatio;
	_projection = glm::perspective(glm::radians(fovDegrees), aspectRatio, 0.1f, 10000.0f);
}

