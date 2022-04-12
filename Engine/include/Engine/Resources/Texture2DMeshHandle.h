#pragma once
#include <string>
namespace Eng {
    enum class Texture2DUsage {
        DIFFUSE = 0,
        SPECULAR = 1,
        NORMAL = 2,
        HEIGHT,
        AMBIENT
    };

    struct Texture2DMeshHandle {
        std::string file_path;
        Texture2DUsage usage{};
    };
}
