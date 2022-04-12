#pragma once

#include "Engine/Core/Types/Vec.h"

namespace Eng
{
	struct Light
	{
		Vec3 ambient;
		Vec3 diffuse;
		Vec3 specular;
		float constant;
		float linear;
		float quadratic;
	};

}
