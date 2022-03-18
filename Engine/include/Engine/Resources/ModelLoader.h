#pragma once
#include "ManagedResources.h"
#include "ResourceCache.h"

namespace Eng {
	class ModelLoader {
	public:
		Model* LoadModel(const std::string& path) const;
	};
}