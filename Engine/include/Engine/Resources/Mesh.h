#pragma once

#include <utility>
#include <vector>
#include <string>
#include "Texture2DMeshHandle.h"
#include "Engine/Core/Types/Vertex.h"

namespace Eng {
    class Mesh {
    public:
        Mesh() = default;

        Mesh(const Mesh &other) = default;

        Mesh(Mesh &&other) noexcept = default;

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture2DMeshHandle> textures;

        Mesh(std::vector<Vertex> v, std::vector<unsigned int> i, std::vector<Texture2DMeshHandle> tb)
                : vertices(std::move(v)), indices(std::move(i)), textures(std::move(tb)) {}
    };
}