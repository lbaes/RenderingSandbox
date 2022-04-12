#pragma once

#include "Light.h"

namespace Eng
{
	struct DirectionalLight : public Light
	{
		Vec3 direction;
	};
}