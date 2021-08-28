#include "matrix_utils.hpp"
#include <cmath>

namespace oom
{
    mat3 tensor_matrix(vec3 const& vec)
    {
        return mat3
        {
            vec.x * vec.x, vec.x * vec.y, vec.x * vec.z,
            vec.y * vec.x, vec.y * vec.y, vec.y * vec.z,
            vec.z * vec.x, vec.z * vec.y, vec.z * vec.z,
        };
    }

    mat3 skew_matrix(vec3 const& vec)
    {
        return mat3
        {
            0,      vec.z, -vec.y,
            -vec.z,     0,  vec.x,
            vec.y, -vec.x,      0,
        };
    }

    mat3 translation_matrix(vec2 const& translation)
    {
        mat3 mat = mat3::identity();

        mat[2] = oom::vec3{ translation.x, translation.y, 1.f };
        //mat[1].z = translation.y;

        return mat;
    }
    
    mat3 rotation_matrix(float const angle)
    {
        mat3 mat = mat3::identity();
        mat[0][0] = mat[1][1] = std::cos(angle);
        mat[1][0] = std::sin(angle);
        mat[0][1] = -mat[1][0];

        return mat;
    }
    
    mat3 scaling_matrix(vec2 const& scale)
    {
        mat3 scaleMatrix = mat3::identity();

        scaleMatrix[0][0] = scale.x;
        scaleMatrix[1][1] = scale.y;
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
        
        mat[3] = vec4{ translation.x, translation.y, translation.z, 1.f };

        return mat;
    }

    mat4 rotation_matrix(float const& angle, vec3 const& dir)
    {
        // axis-angle representation : cos0 I(3x3) + (1-cos0)Tensor + sin0skewedDir

        float cosTheta = std::cos(angle);
        float sinTheta = std::sin(angle);
        float oneMinusTheta = 1.f - cosTheta;

        mat3 cosThetaId { cosTheta };

        mat3 tensor = tensor_matrix(dir);
        mat3 skew = skew_matrix(dir);

        return mat4 { cosThetaId + (1 - cosTheta) * tensor +  sinTheta * skew };


        // mat4 rotationMatrix;
        // set_to_identity(rotationMatrix);

        // float r = maths_util::deg2rad * angle;
        // float c = std::cos(r);
        // float s = std::sin(r);
        // float omc = 1.0f - c;

        // float x = _euler_angles[0];
        // float y = _euler_angles[1];
        // float z = _euler_angles[2];

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

    mat4 rotation_x_matrix(float _angle)
    {
        /**
         * Rotation Matrix on X-axis
         * |   1   0   0   0   |
         * |   0  cos -sin 0   |
         * |   0  sin  cos 0   |
         * |   0   0   0   1   |
         */
        mat4 mat = mat4::identity(); 
        mat[1][1] = mat[2][2] = std::cos(_angle);
        mat[1][2] = std::sin(_angle);
        mat[2][1] = -mat[1][2];

        return mat;
    }

    mat4 rotation_y_matrix(float _angle)
    {
        /**
         * Rotation Matrix on Y-axis
         * |  cos  0  sin  0   |
         * |   0   1   0   0   |
         * | -sin  0  cos  0   |
         * |   0   0   0   1   |
         */
        mat4 mat = mat4::identity();
        mat[0][0] = mat[2][2] = std::cos(_angle);
        mat[2][0] = std::sin(_angle);
        mat[0][2] = -mat[2][0];

        return mat;
    }

    mat4 rotation_z_matrix(float _angle)
    {
        /**
         * Rotation Matrix on Y-axis
         * |  cos -sin 0   0   |
         * |  sin  cos 0   0   |
         * |   0   0   1   0   |
         * |   0   0   0   1   |
         */
        mat4 mat = mat4::identity();
        mat[0][0] = mat[1][1] = std::cos(_angle);
        mat[0][1] = std::sin(_angle);
        mat[1][0] = -mat[0][1];

        return mat;
    }

    mat4 rotation_matrix(const vec3& euler_angles)
    {
        return  rotation_x_matrix(euler_angles.x) 
                * rotation_y_matrix(euler_angles.y)
                * rotation_z_matrix(euler_angles.z);

    }

    mat4 scaling_matrix(const vec3& _scale)
    {
        mat4 scaleMatrix = mat4::identity();

        scaleMatrix[0][0] = _scale.x;
        scaleMatrix[1][1] = _scale.y;
        scaleMatrix[2][2] = _scale.z;
        scaleMatrix[3][3] = 1.0f;

        return scaleMatrix;
    }

    mat4 trs_matrix(const vec3& _translation, const vec3& _euler_angles, const vec3& _scale)
    {
        return translation_matrix(_translation) * rotation_matrix(_euler_angles) * scaling_matrix(_scale);
    }

    mat4 trs_matrix(const vec3& _translation, float const angle, const vec3& direction, const vec3& _scale)
    {
        return translation_matrix(_translation) * rotation_matrix(angle, direction) * scaling_matrix(_scale);
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


    mat4 view(const vec3& forward, const vec3& up, const vec3& position)
    {
        //forward.cross(up);
        
        vec3 right = forward.Cross(up);
        
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

        mat4 mat = mat4::identity();

        // Column 0
        mat[0].x = right.x;
        mat[0].y = right.y;
        mat[0].z = right.z;

        // Column 1
        mat[1].x = up.x;
        mat[1].y = up.y;
        mat[1].z = up.z;
        
        // Column 2
        mat[2].x = -forward.x;
        mat[2].y = -forward.y;
        mat[2].z = -forward.z;

        // Column 3
        mat[3].x = position.x;
        mat[3].y = position.y;
        mat[3].z = position.z;

        return mat;
    }

    mat4 perspective(float const aspect_ratio, float const fov, float const near, float const far)
    {
        mat4 mat = mat4::zero();

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
        float c = (near + far) / (near - far);
        float d = (2.0f * near * far) / (near - far);

        mat[0][0] = a;
        mat[1][1] = b;
        mat[2][2] = c;
        mat[2][3] = -1.0f;
        mat[3][2] = d;

        return mat;
    }

    mat4 ortho(float const left, float const right, float const bottom, float const top, float const near, float const far)
    {
        mat4 mat = mat4::identity();

        mat[0][0] = 2.0f / (right - left);
        mat[1][1] = 2.0f / (top - bottom);
        mat[2][2] = 2.0f / (near - far);

        mat[2][0] = (right + left) / (left - right);
        mat[3][1] = (top + bottom) / (bottom - top);
        mat[3][2] = (far + near) / (near - far);

        mat[3][3] = 1.0f;

        return mat;
    }

    mat4 ortho(float aspect_ratio, float size, float near, float far)
    {
        // [https://en.wikipedia.org/wiki/Orthographic_projection]
        // Most tutorial take the absolute position of the viewing box in the world as the input.
        // So there is a need to translate the box back to the origin.
        // We do not need to do so as we do our calculation assuming that we are already at the origin.

        mat4 mat = mat4::identity();

        float top = size * 0.5f;
        float bottom = -top;
        float right = top * aspect_ratio;
        float left = bottom * aspect_ratio;

        mat[0][0] = 2.0f / (right - left);
        mat[1][1] = 2.0f / (top - bottom);
        mat[2][2] = 2.0f / (far - near);

        return mat;
    }

    
}