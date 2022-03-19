#pragma once
#include "ManagedResources.h"

namespace Eng {
	class ModelLoader {
	public:
		ModelLoader();
		Model LoadModel(const std::string& path) const;
	};
}