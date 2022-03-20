#pragma once
#include "Vec.h"
#include "Mat.h"

namespace Eng {
    class Transform {
    public:
        Transform(const Vec3 &translateVec = {0.0, 0.0, 0.0},
                  const Vec3 &scaleVec = {1.0, 1.0, 1.0},
                  const Vec3 &rotationVec = {1.0, 0.0, 0.0}, float rotationAngle = 0.0f);
    private:
        Mat4 transform_matrix;
    };
}


// Mat4 look_at(const Vec3 &eye, const Vec3 &center, const Vec3 &up);
// Mat4 perspective(float fovy, float aspect, float zNear, float zFar);
// Mat4 translate(const Mat4 &mat, const Vec3 &vec);
// Mat4 rotate(const Mat4 &mat, const Vec3 &vec, float rotationAngle);
// Mat4 scale(const Mat4 &mat, const Vec3 &vec);
