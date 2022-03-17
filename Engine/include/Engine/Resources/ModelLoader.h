#pragma once
#include "ManagedResources.h"
#include "ResourceCache.h"

namespace Eng {
	class ModelLoader {
	public:
		ModelLoader(ResourceCache<Texture2D>* tex_cache);
		ResourceHandle<Model> LoadModel(const std::string& path) const;
	private:
		ResourceCache<Texture2D>* texture_cache;
	};
}