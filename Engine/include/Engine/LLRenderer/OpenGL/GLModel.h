#pragma once
#include <vector>
#include "GLMesh.h"

namespace Eng::ogl {
    struct GLModel {
        std::vector<GLMesh> meshes;
    };
}