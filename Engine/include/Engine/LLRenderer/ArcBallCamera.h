#pragma once
#include "Camera.h"

namespace Eng
{
	class ArcBallCamera : public Camera
	{
	public:
		void UpdateCameraPosByAngle(float angleX, float angleY);
	private:
		float last_angleX;
		float last_angleY;
	};
}


