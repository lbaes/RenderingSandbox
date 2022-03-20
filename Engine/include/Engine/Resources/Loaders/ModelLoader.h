#pragma once
#include <unordered_map>

namespace Eng {
    class Model;
	class ModelLoader {
	public:
		ModelLoader() = default;
        ModelLoader(ModelLoader&&) = delete;
        ModelLoader(const ModelLoader&) = delete;
        void LoadModel(const std::string& path, Model& model);
    };
}