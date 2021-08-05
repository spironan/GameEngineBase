#pragma once

#include "fwd.h"

#include "matrix.h"
#include <optional>

namespace oom 
{
    /*struct matrix_utils 
    {
    public:
        matrix_utils() = delete;*/

        static mat3 translation_matrix(vec2 const& translation);
        static mat3 rotation_matrix(float const angle);
        static mat3 scaling_matrix(vec2 const& scale);
        static mat3 trs_matrix(vec2 const& translation, float const& angle, vec2 const& scale);

        static mat3 translate(mat3& transform, vec2 const& translation);
        static mat3 rotate(mat3& transform, float const angle);
        static mat3 scale(mat3& transform, vec2 const& scale);
        static mat3 trs(mat3& transform, vec2 const& translation, float const& angle, vec2 const& scale);

        /**
            * @brief Generate the homogeneous translation matrix given a translation vector
            *
            * @param translation the translation vectors data that will be used
            * @return mat4 the homogeneous translation matrix
            */
        static mat4 translation_matrix(vec3 const& translation);

        // Axis-Angle Rotation
        static mat4 rotation_matrix(float const& angle, vec3 const& direction);

        /**
            * @brief Generate the homogeneous rotation matrix about the X-axis given angle in radians.
            *
            * @param angle angle to rotate about in radians
            * @return mat4 homogeneous rotation matrix about the X-axis given angle in radians.
            */
        static mat4 rotation_x_matrix(float const angle);

        /**
            * @brief Generate the homogeneous rotation matrix about the Y-axis given angle in radians.
            *
            * @param angle angle to rotate about in radians
            * @return mat4 homogeneous rotation matrix about the Y-axis given angle in radians.
            */
        static mat4 rotation_y_matrix(float const angle);

        /**
            * @brief Generate the homogeneous rotation matrix about the Z-axis given angle in radians.
            *
            * @param angle angle to rotate about in radians
            * @return mat4 homogeneous rotation matrix about the Z-axis given angle in radians.
            */
        static mat4 rotation_z_matrix(float const angle);

        /**
            * @brief Generate the homogeneous rotation matrix about all 3 XYZ-axis
            * given a vector of euler angles rotations.
            *
            * @param euler_angles the euler angles to the XYZ axis
            * @return mat4 homogeneous rotation matrix about the 3 XYZ-axis.
            */
        static mat4 rotation_matrix(vec3 const& euler_angles);

        /**
            * @brief Generate the homogeneous scale matrix given scale vector
            *
            * @param scale the vector representating the scale of the object in XYZ
            * @return mat4
            */
        static mat4 scaling_matrix(vec3 const& scale);

        /**
            * @brief Get the model matrix given translation, rotation and scale.
            *
            * @param translation translation of the object in 3D space
            * @param euler_angles rotation of the object in 3D space
            * @param scale the scale of the object in 3D space
            * @return mat4 the Homogeneous matrix generated
            */
        static mat4 trs_matrix(vec3 const& translation, vec3 const& euler_angles, vec3 const& scale);
        
        static mat4 trs_matrix(vec3 const& translation, float const angle, vec3 const& direction, vec3 const& scale);


        static mat4 translate(mat4& transform, vec3 const& translation);
        static mat4 rotate(mat4& transform, float const& angle, vec3 const& direction);
        static mat4 rotate(mat4& transform, vec3 const& euler_angles);
        static mat4 rotate_x(mat4& transform, float const angle);
        static mat4 rotate_y(mat4& transform, float const angle);
        static mat4 rotate_z(mat4& transform, float const angle);
        static mat4 scale(mat4& transform, vec3 const& scale);
        static mat4 trs(mat4& transform, vec3 const& translation, vec3 const& euler_angles, vec3 const& scale);
        static mat4 trs(mat4& transform, vec3 const& translation, float const angle, vec3 const& direction, vec3 const& scale);


        /**
            * @brief Get the view matrix given the cameras orientation and position
            *
            * @param forward the forward vector
            * @param up the up vector
            * @param position the position in 3D space
            * @return mat4 the view matrix in 3D space
            */
        static mat4 view(vec3 const& forward, vec3 const& up, vec3 const& position);

        static mat4 perspective(float const aspect_ratio, float const fov, float const near_plane, float const far_plane);

        static mat4 orthographic(float const left, float const right, float const top, float const bottom, float const near_plane, float const far_plane);

        static mat4 orthographic(float const aspect_ratio, float const other_size, float const near_plane, float const far_plane);

        /**
            * @brief Get the minor square matrix given a matrix of a bigger size
            *
            * @tparam Size the size of the original matrix
            * @param matrix the original matrix
            * @param _cofactor_col the column that will not be used the minor matrix
            * @param _cofactor_row the row that will not be used in the minor matrix
            * @return matrix<Size - 1, Size - 1> the minor square matrix of size - 1
            */
        template<size_t Size>
        static mat<Size - 1>
        minor(const mat<Size>& mat, size_t cofactor_col, size_t cofactor_row)
        {
            matrix<Size - 1, Size - 1> result;
            for (float major_col = 0, minor_col = 0; major_col < Size; ++major_col) {
                if (major_col == cofactor_col) continue;
                for (float major_row = 0, minor_row = 0; major_row < Size; ++major_row) {
                    if (major_row == cofactor_row) continue;
                    result[minor_col][minor_row] = mat[major_col][major_row];
                    ++minor_row;
                }
                ++minor_col;
            }
        }

        /**
            * @brief Get the determinant of given square matrix
            *
            * @tparam Size the size of the matrix
            * @param matrix the matrix that will be used
            * @return float the determinant of the matrix
            */
        template<size_t Size>
        static float determinant(const mat<Size>& mat)
        {
            float det = 0.0f;
            for (size_t column = 0; column < Size; ++column) {
                float cofactor = mat[column][0];
                //determine sign-ness of that iteration
                cofactor *= 1 + ((column & 1) * -2);

                matrix<Size - 1, Size - 1> minor_mat = minor(mat, column, 0);
                det += determinant(minor_mat) * cofactor;
            }

            return det;
        }

        /**
            * @brief Get the determinant of a 2 by 2 matrix
            *
            * @param matrix the matrix that will be used
            * @return float the determinant of the matrix
            */
        static float determinant(const mat2& matrix)
        {
            return (matrix[0][0] * matrix[1][1]) - (matrix[1][0] * matrix[0][1]);
        };

        /**
            * @brief Get the determinant of a 1 by 1 matrix
            *
            * @param matrix the matrix that will be used
            * @return float the determinant of the matrix
            */
        static float determinant(const mat1& matrix) { return matrix[0][0]; };

        /**
            * @brief Get the cofactor matrix of a given matrix
            *
            * @tparam Size the size of the matrix
            * @param matrix the original matrix
            * @return mat<Size> the cofactor matrix generated from the original matrix
            */
        template<size_t Size>
        static mat<Size> cofactor(const mat<Size>& matrix)
        {
            mat<Size> result;

            for (size_t column = 0; column < Size; ++column) {
                for (size_t row = 0; row < Size; ++row) {
                    float determinant = determinant(minor(matrix, column, row));
                    result[column][row] = determinant * (1 + (column ^ row) * -2);
                }
            }

            return result;
        }

        /**
            * @brief Get the cofactor matrix of a 1 by 1 matrix
            *
            * @param matrix the original matrix
            * @return matrix<1, 1> the cofactor matrix generated from the original matrix
            */
        static mat1 cofactor(const mat1& matrix) { return matrix; }

        /**
            * @brief Get the adjugate matrix of a given matrix
            *
            * @tparam Size the size of the original matrix
            * @param matrix the original matrix
            * @return mat<Size> the adjugate matrix gernerated from the original matrix
            */
        template<size_t Size>
        static mat<Size> adjugate(const mat<Size>& matrix)
        {
            return cofactor(matrix).transposed();
        }

        /**
            * @brief Get the inverse matrix of given matrix
            *
            * @tparam Size the size of the original matrix
            * @param matrix the matrix that will be used to find its inverse
            * @return an optional result depending on whether an inverse exist.
            * mat<Size> the inverse of the given square matrix
            */
        template<size_t Size>
        static std::optional<mat<Size>> inverse(const mat<Size>& matrix)
        {
            if (!has_inverse(matrix)) return std::nullopt;

            return 1.0f / determinant(matrix) * adjugate(matrix);
        }

        /**
            * @brief Get the inverse matrix of given 4 by 4 matrix
            *
            * @param matrix the original 4 by 4 matrix
            * @return mat4 the resultant inverse
            */
        static mat4 inverse(const mat4& matrix);

        /**
            * @brief boolean to test if an inverse exist
            *
            * @tparam Size the size of the matrix
            * @param matrix the matrix that will be used to test
            * @return true if the determinant of the matrix is not 0
            * @return false if the determinant of the matrix is 0
            */
        template<size_t Size>
        static bool has_inverse(const mat<Size>& matrix)
        {
            return !approx_equal(determinant(matrix), 0.0f);
        }

        /*template<size_t Size>
        static constexpr mat<Size> identity() 
        {
            mat<Size> result;
            for (size_t i = 0; i < Size; ++i) {
                result[i][i] = 1.0f;
            }
            return result;
        }*/

        /*template<size_t Size>
        void set_to_identity(mat<Size> const& matrix) { matrix = identity<Size>(); }*/
        
        /*template<size_t Size>
        void transpose(mat<Size> const& matrix) { matrix = matrix.transposed(); }*/

    /*};*/
}