#pragma once
#include "Engine/Core/Types/Vec.h"
#include "Light.h"

namespace Eng
{
	struct PointLight : public Light
	{
		Vec3 position;
	};
}