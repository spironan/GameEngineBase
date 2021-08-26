//#include "pch.h"

#include "matrix_utils.h"
#include <cmath>

namespace oom
{
    mat3 translation_matrix(vec2 const& translation)
    {
        mat3 mat = mat3::identity();

        mat[2][0] = translation[0];
        mat[2][1] = translation[1];

        return mat;
    }
    
    mat3 rotation_matrix(float const angle)
    {
        mat3 mat = mat3::identity();
        mat[0][0] = mat[1][1] = std::cos(angle);
        mat[0][1] = std::sin(angle);
        mat[1][0] = -mat[0][1];

        return mat;
    }
    
    mat3 scaling_matrix(vec2 const& scale)
    {
        mat3 scaleMatrix = mat3::identity();

        scaleMatrix[0][0] = scale[0];
        scaleMatrix[1][1] = scale[1];
        scaleMatrix[2][2] = 1.f;

        return scaleMatrix;
    }
    
    mat3 trs_matrix(vec2 const& translation, float const& angle, vec2 const& scale)
    {
        return translation_matrix(translation) * rotation_matrix(angle) * scaling_matrix(scale);
    }

    mat3 translate(mat3& transform, vec2 const& translation)
    {
        return translation_matrix(translation) * transform;
    }

    mat3 rotate(mat3& transform, float const angle)
    {
        return rotation_matrix(angle) * transform;
    }

    mat3 scale(mat3& transform, vec2 const& scale)
    {
        return scaling_matrix(scale) * transform;
    }

    mat3 trs(mat3& transform, vec2 const& translation, float const& angle, vec2 const& scale)
    {
        return translation_matrix(translation) * rotation_matrix(angle) * scaling_matrix(scale) * transform;
    }

    
    mat4 translation_matrix(const vec3& translation)
    {
        mat4 mat = mat4::identity();
        
        mat[3][0] = translation[0];
        mat[3][1] = translation[1];
        mat[3][2] = translation[2];

        return mat;
    }

    mat4 rotation_matrix(float const& angle, vec3 const& dir)
    {
        // axis-angle representation : cos0 I(3x3) + (1-cos0)dir*dirTransposed + sin0skewedDir

        float cosTheta = std::cos(angle);
        float sinTheta = std::sin(angle);
        float oneMinusTheta = 1.f - cosTheta;

        mat3 cosThetaId { cosTheta };

        /*mat3 tensorMatrix
        {
            dir.x()() * dir.x()(), dir.x()() * dir.y()(), dir.x()() * dir.z()(),
            dir.y()() * dir.x()(), dir.y()() * dir.y()(), dir.y()() * dir.z()(),
            dir.z()() * dir.x()(), dir.z()() * dir.y()(), dir.z()() * dir.z()()
        };

        mat3 skewMatrix
        {
             0.f,    -dir.z()(),  dir.y()(),
             dir.z()(),  0.f,    -dir.x()(),
            -dir.y()(), dir.x()(),   0.f
        };*/

        return mat4 { cosThetaId /*+ tensorMatrix + skewMatrix*/ };


        // mat4 rotationMatrix;
        // set_to_identity(rotationMatrix);

        // float r = maths_util::deg2rad * angle;
        // float c = std::cos(r);
        // float s = std::sin(r);
        // float omc = 1.0f - c;

        // float x = euler_angles[0];
        // float y = euler_angles[1];
        // float z = euler_angles[2];

        // rotationMatrix[0][0] = x * omc + c;
        // rotationMatrix[0][1] = y * x * omc + z * s;
        // rotationMatrix[0][2] = x * z * omc - y * s;

        // rotationMatrix[1][0] = x * y * omc - z * s;
        // rotationMatrix[1][1] = y * omc + c;
        // rotationMatrix[1][2] = y * z * omc + x * s;

        // rotationMatrix[2][0] = x * z * omc + y * s;
        // rotationMatrix[2][1] = y * z * omc - x * s;
        // rotationMatrix[2][2] = z * omc + c;

        // return rotationMatrix;
    }

    mat4 rotation_x_matrix(float angle)
    {
        /**
         * Rotation Matrix on X-axis
         * |   1   0   0   0   |
         * |   0  cos -sin 0   |
         * |   0  sin  cos 0   |
         * |   0   0   0   1   |
         */
        mat4 mat = mat4::identity(); 
        mat[1][1] = mat[2][2] = std::cos(angle);
        mat[1][2] = std::sin(angle);
        mat[2][1] = -mat[1][2];

        return mat;
    }

    mat4 rotation_y_matrix(float angle)
    {
        /**
         * Rotation Matrix on Y-axis
         * |  cos  0  sin  0   |
         * |   0   1   0   0   |
         * | -sin  0  cos  0   |
         * |   0   0   0   1   |
         */
        mat4 mat = mat4::identity();
        mat[0][0] = mat[2][2] = std::cos(angle);
        mat[2][0] = std::sin(angle);
        mat[0][2] = -mat[2][0];

        return mat;
    }

    mat4 rotation_z_matrix(float angle)
    {
        /**
         * Rotation Matrix on Y-axis
         * |  cos -sin 0   0   |
         * |  sin  cos 0   0   |
         * |   0   0   1   0   |
         * |   0   0   0   1   |
         */
        mat4 mat = mat4::identity();
        mat[0][0] = mat[1][1] = std::cos(angle);
        mat[0][1] = std::sin(angle);
        mat[1][0] = -mat[0][1];

        return mat;
    }

    mat4 rotation_matrix(const vec3& euler_angles)
    {
        return  rotation_x_matrix(euler_angles[0]) 
                * rotation_y_matrix(euler_angles[1])
                * rotation_z_matrix(euler_angles[2]);

    }

    mat4 scaling_matrix(const vec3& scale)
    {
        mat4 scaleMatrix;

        scaleMatrix[0][0] = scale[0];
        scaleMatrix[1][1] = scale[1];
        scaleMatrix[2][2] = scale[2];
        scaleMatrix[3][3] = 1.0f;

        return scaleMatrix;
    }

    mat4 trs_matrix(const vec3& translation, const vec3& euler_angles, const vec3& scale)
    {
        return translation_matrix(translation) * rotation_matrix(euler_angles) * scaling_matrix(scale);
    }

    mat4 trs_matrix(const vec3& translation, float const angle, const vec3& direction, const vec3& scale)
    {
        return translation_matrix(translation) * rotation_matrix(angle, direction) * scaling_matrix(scale);
    }

    mat4 translate(mat4& transform, vec3 const& translation)
    {
        return translation_matrix(translation) * transform;
    }

    mat4 rotate(mat4& transform, float const& angle, vec3 const& direction)
    {
        return rotation_matrix(angle, direction) * transform;
    }

    mat4 rotate_x(mat4& transform, float const angle)
    {
        return rotation_x_matrix(angle) * transform;
    }

    mat4 rotate_y(mat4& transform, float const angle)
    {
        return rotation_y_matrix(angle) * transform;
    }

    mat4 rotate_z(mat4& transform, float const angle)
    {
        return rotation_z_matrix(angle) * transform;
    }

    mat4 rotate(mat4& transform, vec3 const& euler_angles)
    {
        return rotation_matrix(euler_angles) * transform;
    }

    mat4 scale(mat4& transform, vec3 const& scale)
    {
        return scaling_matrix(scale) * transform;
    }

    mat4 trs(mat4& transform, vec3 const& translation, vec3 const& euler_angles, vec3 const& scale)
    {
        return translation_matrix(translation) * rotation_matrix(euler_angles) * scaling_matrix(scale) * transform;
    }

    mat4 trs(mat4& transform, vec3 const& translation, float const angle, vec3 const& direction, vec3 const& scale)
    {
        return translation_matrix(translation) * rotation_matrix(angle, direction) * scaling_matrix(scale) * transform;
    }


    mat4 view(const vec3& _forward, const vec3& _up, const vec3& _position)
    {
        //_forward.cross(_up);
        
        vec3 right = _forward.cross(_up);
        
        /**
         * translation * orientation matrix  = homogeneous matrix
         * [ 1 0 0 T1 ][ R11  R12  R13  0 ]    [ R11 R12 R13 T1 ]
         * [ 0 1 0 T2 ][ R21  R22  R23  0 ]  = [ R21 R22 R23 T2 ]
         * [ 0 0 1 T3 ][ R31  R32  R33  0 ]    [ R31 R32 R33 T3 ]
         * [ 0 0 0 1  ][  0    0    0   1 ]    [  0   0   0  1  ]
         *
         * [  Right   trans ]
         * [  Up      trans ]
         * [ -Forward trans ]
         * [  0   0   0   1  ]
         */

        mat4 mat;
        mat[3][3] = 1.0f;

        // Column 0
        mat[0][0] = right[0];
        mat[0][1] = _up[0];
        mat[0][2] = -_forward[0];

        // Column 1
        mat[1][0] = right[1];
        mat[1][1] = _up[1];
        mat[1][2] = -_forward[1];

        // Column 2
        mat[2][0] = right[2];
        mat[2][1] = _up[2];
        mat[2][2] = -_forward[2];

        mat[3][0] = _position[0];
        mat[3][1] = _position[1];
        mat[3][2] = _position[2];

        return mat;
    }

    mat4 perspective(float aspect_ratio, float fov, float near_plane, float far_plane)
    {
        mat4 mat = mat4::identity();

        /**
         * AR  = Aspect Ratio
         * FOV = Field of View
         * N   = Near Plane
         * F   = Far Plane
         *
         * Perspective Matrix:
         * | 1/(tan(FOV/2) * AR)         0                0              0       |
         * |          0            1/tan(FOV/2)           0              0       |
         * |          0                  0          (N+F)/(N-F)   (2*F*N)/(N-F)  |
         * |          0                  0               -1              0       |
         * */

        float b = 1.0f / std::tan(0.5f * fov);
        float a = b / aspect_ratio;
        float c = (near_plane + far_plane) / (near_plane - far_plane);
        float d = (2.0f * near_plane * far_plane) / (near_plane - far_plane);

        mat[0][0] = a;
        mat[1][1] = b;
        mat[2][2] = c;
        mat[2][3] = -1.0f;
        mat[3][2] = d;

        return mat;
    }

    mat4 orthographic(float left, float right, float top, float bottom, float near_plane, float far_plane)
    {
        mat4 mat = mat4::identity();

        mat[0][0] = 2.0f / (right - left);
        mat[1][1] = 2.0f / (top - bottom);
        mat[2][2] = 2.0f / (near_plane - far_plane);

        mat[3][0] = (right + left) / (left - right);
        mat[3][1] = (top + bottom) / (bottom - top);
        mat[3][2] = (far_plane + near_plane) / (near_plane - far_plane);

        mat[3][3] = 1.0f;

        return mat;
    }

    mat4 orthographic(float aspect_ratio, float ortho_size, float near_plane, float far_plane)
    {
        // [https://en.wikipedia.org/wiki/Orthographic_projection]
        // Most tutorial take the absolute position of the viewing box in the world as the input.
        // So there is a need to translate the box back to the origin.
        // We do not need to do so as we do our calculation assuming that we are already at the origin.

        mat4 mat = mat4::identity();

        float top = ortho_size * 0.5f;
        float bottom = -top;
        float right = top * aspect_ratio;
        float left = bottom * aspect_ratio;

        mat[0][0] = 2.0f / (right - left);
        mat[1][1] = 2.0f / (top - bottom);
        mat[2][2] = 2.0f / (far_plane - near_plane);

        return mat;
    }

    mat4 inverse(const mat4& matrix)
    {
        mat4 mat;

        mat[0][0] =
            matrix[0][5] * matrix[0][10] * matrix[0][15] -
            matrix[0][5] * matrix[0][11] * matrix[0][14] -
            matrix[0][9] * matrix[0][6] * matrix[0][15] +
            matrix[0][9] * matrix[0][7] * matrix[0][14] +
            matrix[0][13] * matrix[0][6] * matrix[0][11] -
            matrix[0][13] * matrix[0][7] * matrix[0][10];

        mat[0][4] =
            -matrix[0][4] * matrix[0][10] * matrix[0][15] +
            matrix[0][4] * matrix[0][11] * matrix[0][14] +
            matrix[0][8] * matrix[0][6] * matrix[0][15] -
            matrix[0][8] * matrix[0][7] * matrix[0][14] -
            matrix[0][12] * matrix[0][6] * matrix[0][11] +
            matrix[0][12] * matrix[0][7] * matrix[0][10];

        mat[0][8] =
            matrix[0][4] * matrix[0][9] * matrix[0][15] -
            matrix[0][4] * matrix[0][11] * matrix[0][13] -
            matrix[0][8] * matrix[0][5] * matrix[0][15] +
            matrix[0][8] * matrix[0][7] * matrix[0][13] +
            matrix[0][12] * matrix[0][5] * matrix[0][11] -
            matrix[0][12] * matrix[0][7] * matrix[0][9];

        mat[0][12] =
            -matrix[0][4] * matrix[0][9] * matrix[0][14] +
            matrix[0][4] * matrix[0][10] * matrix[0][13] +
            matrix[0][8] * matrix[0][5] * matrix[0][14] -
            matrix[0][8] * matrix[0][6] * matrix[0][13] -
            matrix[0][12] * matrix[0][5] * matrix[0][10] +
            matrix[0][12] * matrix[0][6] * matrix[0][9];

        mat[0][1] =
            -matrix[0][1] * matrix[0][10] * matrix[0][15] +
            matrix[0][1] * matrix[0][11] * matrix[0][14] +
            matrix[0][9] * matrix[0][2] * matrix[0][15] -
            matrix[0][9] * matrix[0][3] * matrix[0][14] -
            matrix[0][13] * matrix[0][2] * matrix[0][11] +
            matrix[0][13] * matrix[0][3] * matrix[0][10];

        mat[0][5] =
            matrix[0][0] * matrix[0][10] * matrix[0][15] -
            matrix[0][0] * matrix[0][11] * matrix[0][14] -
            matrix[0][8] * matrix[0][2] * matrix[0][15] +
            matrix[0][8] * matrix[0][3] * matrix[0][14] +
            matrix[0][12] * matrix[0][2] * matrix[0][11] -
            matrix[0][12] * matrix[0][3] * matrix[0][10];

        mat[0][9] =
            -matrix[0][0] * matrix[0][9] * matrix[0][15] +
            matrix[0][0] * matrix[0][11] * matrix[0][13] +
            matrix[0][8] * matrix[0][1] * matrix[0][15] -
            matrix[0][8] * matrix[0][3] * matrix[0][13] -
            matrix[0][12] * matrix[0][1] * matrix[0][11] +
            matrix[0][12] * matrix[0][3] * matrix[0][9];

        mat[0][13] =
            matrix[0][0] * matrix[0][9] * matrix[0][14] -
            matrix[0][0] * matrix[0][10] * matrix[0][13] -
            matrix[0][8] * matrix[0][1] * matrix[0][14] +
            matrix[0][8] * matrix[0][2] * matrix[0][13] +
            matrix[0][12] * matrix[0][1] * matrix[0][10] -
            matrix[0][12] * matrix[0][2] * matrix[0][9];

        mat[0][2] =
            matrix[0][1] * matrix[0][6] * matrix[0][15] -
            matrix[0][1] * matrix[0][7] * matrix[0][14] -
            matrix[0][5] * matrix[0][2] * matrix[0][15] +
            matrix[0][5] * matrix[0][3] * matrix[0][14] +
            matrix[0][13] * matrix[0][2] * matrix[0][7] -
            matrix[0][13] * matrix[0][3] * matrix[0][6];

        mat[0][6] =
            -matrix[0][0] * matrix[0][6] * matrix[0][15] +
            matrix[0][0] * matrix[0][7] * matrix[0][14] +
            matrix[0][4] * matrix[0][2] * matrix[0][15] -
            matrix[0][4] * matrix[0][3] * matrix[0][14] -
            matrix[0][12] * matrix[0][2] * matrix[0][7] +
            matrix[0][12] * matrix[0][3] * matrix[0][6];

        mat[0][10] =
            matrix[0][0] * matrix[0][5] * matrix[0][15] -
            matrix[0][0] * matrix[0][7] * matrix[0][13] -
            matrix[0][4] * matrix[0][1] * matrix[0][15] +
            matrix[0][4] * matrix[0][3] * matrix[0][13] +
            matrix[0][12] * matrix[0][1] * matrix[0][7] -
            matrix[0][12] * matrix[0][3] * matrix[0][5];

        mat[0][14] =
            -matrix[0][0] * matrix[0][5] * matrix[0][14] +
            matrix[0][0] * matrix[0][6] * matrix[0][13] +
            matrix[0][4] * matrix[0][1] * matrix[0][14] -
            matrix[0][4] * matrix[0][2] * matrix[0][13] -
            matrix[0][12] * matrix[0][1] * matrix[0][6] +
            matrix[0][12] * matrix[0][2] * matrix[0][5];

        mat[0][3] =
            -matrix[0][1] * matrix[0][6] * matrix[0][11] +
            matrix[0][1] * matrix[0][7] * matrix[0][10] +
            matrix[0][5] * matrix[0][2] * matrix[0][11] -
            matrix[0][5] * matrix[0][3] * matrix[0][10] -
            matrix[0][9] * matrix[0][2] * matrix[0][7] +
            matrix[0][9] * matrix[0][3] * matrix[0][6];

        mat[0][7] =
            matrix[0][0] * matrix[0][6] * matrix[0][11] -
            matrix[0][0] * matrix[0][7] * matrix[0][10] -
            matrix[0][4] * matrix[0][2] * matrix[0][11] +
            matrix[0][4] * matrix[0][3] * matrix[0][10] +
            matrix[0][8] * matrix[0][2] * matrix[0][7] -
            matrix[0][8] * matrix[0][3] * matrix[0][6];

        mat[0][11] =
            -matrix[0][0] * matrix[0][5] * matrix[0][11] +
            matrix[0][0] * matrix[0][7] * matrix[0][9] +
            matrix[0][4] * matrix[0][1] * matrix[0][11] -
            matrix[0][4] * matrix[0][3] * matrix[0][9] -
            matrix[0][8] * matrix[0][1] * matrix[0][7] +
            matrix[0][8] * matrix[0][3] * matrix[0][5];

        mat[0][15] =
            matrix[0][0] * matrix[0][5] * matrix[0][10] -
            matrix[0][0] * matrix[0][6] * matrix[0][9] -
            matrix[0][4] * matrix[0][1] * matrix[0][10] +
            matrix[0][4] * matrix[0][2] * matrix[0][9] +
            matrix[0][8] * matrix[0][1] * matrix[0][6] -
            matrix[0][8] * matrix[0][2] * matrix[0][5];

        float determinant =
            matrix[0][0] * mat[0][0]
            + matrix[0][1] * mat[0][4]
            + matrix[0][2] * mat[0][8]
            + matrix[0][3] * mat[0][12];
        determinant = 1.0f / determinant;

        for (size_t i = 0; i < 16; ++i) {
            mat[0][i] *= determinant;
        }

        return mat;
    }
}