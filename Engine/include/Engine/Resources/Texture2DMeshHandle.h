#pragma once
#include <string>
namespace Eng {
    enum class Texture2DUsage {
        DIFFUSE,
        SPECULAR,
        NORMAL,
        HEIGHT,
    };

    struct Texture2DMeshHandle {
        std::string file_path;
        Texture2DUsage usage{};
    };
}
