#include "ArcBallCamera.h"
#include "glm/ext/matrix_transform.hpp"

void Eng::ArcBallCamera::UpdateCameraView(float angleX, float angleY)
{
	// position and pivot as Vec4
	Vec4 position = {_camera_pos.x,_camera_pos.y,_camera_pos.z, 1.0f};
	Vec4 pivot = {_camera_target.x,_camera_target.y,_camera_target.z, 1.0f};
	Vec3 target_direction = glm::normalize(_camera_pos - _camera_target);
	Vec3 camera_right = glm::normalize(glm::cross(_camera_up,  target_direction));

	// if target_direction is the same as up vector, prevent camera from flickering
	float p = glm::dot(target_direction, _camera_up);
	if (p > 0.98 && angleY < 0 || p < -0.98 && angleY > 0){
		angleY = 0;
	}

	// rotate around X
	Mat4 rotationX(1.0f);
	rotationX = glm::rotate(rotationX, glm::radians(angleX), _camera_up);
	position = (rotationX * (position - pivot)) + pivot;

	// rotate around Y
	Mat4 rotationY(1.0f);
	rotationY = glm::rotate(rotationY, glm::radians(angleY), camera_right);
	Vec3 finalPosition = (rotationY * (position - pivot)) + pivot;

	UpdateCamera(finalPosition, _camera_target, _camera_up);
}
