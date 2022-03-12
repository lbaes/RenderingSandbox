#pragma once
#include <Engine/Utility/Singleton.h>
#include <Engine/Resources/ResourceCache.h>
#include <Engine/Resources/ManagedResources.h>

namespace Eng {
	class ResourceManager : public Singleton<ResourceManager> {
	public:
		ResourceManager() {

		}

	private:
		ResourceCache<Texture2D> texture_cache;
	};
}