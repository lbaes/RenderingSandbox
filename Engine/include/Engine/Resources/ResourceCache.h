#pragma once
#include <unordered_map>
#include <string>

namespace Eng {
	template <class Resource>
	class ResourceCache {
	public:
		Resource* Fetch(const std::string& path) {
			if (!loaded_resources.contains(path)) {
				Reload(path);
			};
			return loaded_resources.at(path);
		}

		void Reload(const std::string& path) {
			loaded_resources[path] = Resource::LoadFromDisk(path);
		}

		~ResourceCache() {
			for (auto& resource : loaded_resources) {
				delete resource.second;
			}
		}

	private:
		std::unordered_map<std::string, Resource*> loaded_resources;
	};
}