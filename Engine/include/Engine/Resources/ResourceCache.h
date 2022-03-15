#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "ResourceHandle.h"

namespace Eng {
	template <class Resource>
	class ResourceCache {
	public:
		ResourceHandle<Resource> Fetch(const std::string& path) {
			if (!loaded_resources.contains(path)) {
				Reload(path);
			};
			ResourceHandle<Resource> handle;
			handle.resource = loaded_resources.at(path);
			return handle;
		}

		void Reload(const std::string& path) {
			auto resource = Resource::LoadFromDisk(path);
			loaded_resources.insert(std::make_pair(path, std::shared_ptr<Resource>(resource)));
		}

	private:
		std::unordered_map<std::string, std::shared_ptr<Resource>> loaded_resources;
	};
}
