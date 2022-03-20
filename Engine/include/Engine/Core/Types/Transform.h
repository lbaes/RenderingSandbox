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
                  const Vec3 &rotationVec = {0.0, 1.0, 0.0}, float rotationAngle = 0.0f);
        inline Mat4 GetTransformation() const;
    private:
        Mat4 transform_matrix;
    };

    inline Transform::Transform(const Vec3 &translateVec, const Vec3 &scaleVec, const Vec3 &rotationVec, float rotationAngle) {
        transform_matrix = glm::mat4(1.0f);
        transform_matrix = glm::translate(transform_matrix, translateVec);
        transform_matrix = glm::rotate(transform_matrix, rotationAngle, translateVec);
        transform_matrix = glm::scale(transform_matrix, scaleVec);
    }

    inline Mat4 Transform::GetTransformation() const {
        return transform_matrix;
    }
}


// Mat4 look_at(const Vec3 &eye, const Vec3 &center, const Vec3 &up);
// Mat4 perspective(float fovy, float aspect, float zNear, float zFar);
// Mat4 translate(const Mat4 &mat, const Vec3 &vec);
// Mat4 rotate(const Mat4 &mat, const Vec3 &vec, float rotationAngle);
// Mat4 scale(const Mat4 &mat, const Vec3 &vec);
