#pragma once
#include "Vec.h"
#include "Mat.h"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace Eng {
    class Transform {
    public:
        inline Transform(const Vec3 &translateVec = {0.0, 0.0, 0.0},
                  const Vec3 &scaleVec = {1.0, 1.0, 1.0},
                  const Vec3 &rotationVec = {0.0, 0.0, 0.0});
        inline Mat4 GetTransformation() const;
    private:
        Mat4 transform_matrix;
    };

    inline Transform::Transform(const Vec3 &translateVec, const Vec3 &scaleVec, const Vec3 &rotationVec)
        : transform_matrix{1.0f} {
        transform_matrix = glm::translate(transform_matrix, translateVec);
        transform_matrix = glm::rotate(transform_matrix, glm::radians(rotationVec.x), {1.0f, 0.0f, 0.0f});
        transform_matrix = glm::rotate(transform_matrix, glm::radians(rotationVec.y), {0.0f, 1.0f, 0.0f});
        transform_matrix = glm::rotate(transform_matrix, glm::radians(rotationVec.z), {0.0f, 0.0f, 1.0f});
        transform_matrix = glm::scale(transform_matrix, scaleVec);
    }

    inline Mat4 Transform::GetTransformation() const {
        return transform_matrix;
    }
}
