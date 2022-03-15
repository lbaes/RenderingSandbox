#pragma once
#include <string>
#include <Engine/Utility/Singleton.h>
#include <Engine/Resources/ResourceCache.h>
#include <Engine/Resources/ManagedResources.h>
#include "ResourceHandle.h"

namespace Eng {
	class ResourceManager : public Singleton<ResourceManager> {
	public:
		ResourceManager() {

		}

		ResourceHandle<Texture2D> LoadTexture(const std::string& filename) {
			return texture_cache.Fetch(filename);
		};

	private:
		ResourceCache<Texture2D> texture_cache;
		ResourceCache<Shader> shader_cache;
	};
}