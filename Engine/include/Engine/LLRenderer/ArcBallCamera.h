#pragma once
#include "Camera.h"

namespace Eng
{
	class ArcBallCamera : public Camera
	{
	public:
		void UpdateCameraView(float angleX, float angleY);
	};
}


