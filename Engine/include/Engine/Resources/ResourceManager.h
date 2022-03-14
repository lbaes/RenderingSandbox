#pragma once
#include <string>
#include <Engine/Utility/Singleton.h>
#include <Engine/Resources/ResourceCache.h>
#include <Engine/Resources/ManagedResources.h>

namespace Eng {
	class ResourceManager : public Singleton<ResourceManager> {
	public:
		ResourceManager() {

		}

		Texture2D LoadTexture(const std::string& filename);

	private:
		ResourceCache<Texture2D> texture_cache;
	};
}