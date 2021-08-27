#pragma once

//#include "fwd.h"
#include "vector_defines.hpp"
#include "matrix_defines.hpp"

#include <optional>

namespace oom 
{
    mat3 tensor_matrix(vec3 const& vec);
    mat3 skew_matrix(vec3 const& vec);

    mat3 translation_matrix(vec2 const& translation);
    mat3 rotation_matrix(float const angle);
    mat3 scaling_matrix(vec2 const& scale);
    mat3 trs_matrix(vec2 const& translation, float const& angle, vec2 const& scale);

    mat3 translate(mat3& transform, vec2 const& translation);
    mat3 rotate(mat3& transform, float const angle);
    mat3 scale(mat3& transform, vec2 const& scale);
    mat3 trs(mat3& transform, vec2 const& translation, float const& angle, vec2 const& scale);

    mat4 translation_matrix(vec3 const& translation);
    mat4 rotation_matrix(float const& angle, vec3 const& direction);    // Axis-Angle Rotation
    mat4 rotation_x_matrix(float const angle);
    mat4 rotation_y_matrix(float const angle);
    mat4 rotation_z_matrix(float const angle);
    mat4 rotation_matrix(vec3 const& euler_angles);
    mat4 scaling_matrix(vec3 const& scale);
    mat4 trs_matrix(vec3 const& translation, vec3 const& euler_angles, vec3 const& scale);
    mat4 trs_matrix(vec3 const& translation, float const angle, vec3 const& direction, vec3 const& scale);


    mat4 translate(mat4& transform, vec3 const& translation);
    mat4 rotate(mat4& transform, float const& angle, vec3 const& direction);
    mat4 rotate(mat4& transform, vec3 const& euler_angles);
    mat4 rotate_x(mat4& transform, float const angle);
    mat4 rotate_y(mat4& transform, float const angle);
    mat4 rotate_z(mat4& transform, float const angle);
    mat4 scale(mat4& transform, vec3 const& scale);
    mat4 trs(mat4& transform, vec3 const& translation, vec3 const& euler_angles, vec3 const& scale);
    mat4 trs(mat4& transform, vec3 const& translation, float const angle, vec3 const& direction, vec3 const& scale);


    mat4 view(vec3 const& forward, vec3 const& up, vec3 const& position);
    mat4 perspective(float const aspect_ratio, float const fov, float const near_plane, float const far_plane);
    mat4 ortho(float const left, float const right, float const bottom, float const top, float const near_plane, float const far_plane);
    mat4 ortho(float const aspect_ratio, float const size, float const near_plane, float const far_plane);

    /**
        * @brief Get the minor square matrix given a matrix of a bigger size
        *
        * @tparam Size the size of the original matrix
        * @param matrix the original matrix
        * @param _cofactor_col the column that will not be used the minor matrix
        * @param _cofactor_row the row that will not be used in the minor matrix
        * @return matrix<Size - 1, Size - 1> the minor square matrix of size - 1
        */
    template<length_t Size, typename T>
    mat<Size - 1, Size - 1, T>
    minor(mat<Size, Size, T> const& matrix, length_t cofactor_col, length_t cofactor_row)
    {
        mat<Size - 1, Size - 1, T> result;

        for (float major_col = 0, minor_col = 0; major_col < Size; ++major_col) 
        {
            if (major_col == cofactor_col) continue;

            for (float major_row = 0, minor_row = 0; major_row < Size; ++major_row) 
            {
                if (major_row == cofactor_row) continue;

                result[minor_col][minor_row] = matrix[major_col][major_row];
                ++minor_row;
            }
            ++minor_col;
        }

        return result;
    }

    /**
        * @brief Get the determinant of given square matrix
        *
        * @tparam Size the size of the matrix
        * @param matrix the matrix that will be used
        * @return float the determinant of the matrix
        */
    template<length_t Size, typename T>
    float determinant(mat<Size, Size, T> const& matrix)
    {
        float det = 0.0f;
        for (length_t column = 0; column < Size; ++column)
        {
            float cofactor = matrix[column][0];
            //determine sign-ness of that iteration
            cofactor *= 1 + ((column & 1) * -2);
            //mat<Size - 1, Size - 1, T> shrank_mat = matrix;
            mat<Size - 1, Size - 1, T> minor_mat = minor<Size, T>(matrix, column, 0);
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
    template<>
    float determinant(mat<2, 2, float> const& matrix)
    {
        return (matrix[0][0] * matrix[1][1]) - (matrix[1][0] * matrix[0][1]);
    };

    /**
        * @brief Get the determinant of a 1 by 1 matrix
        *
        * @param matrix the matrix that will be used
        * @return float the determinant of the matrix
        */
    template<>
    float determinant(mat<1, 1, float> const& matrix) { return matrix[0][0]; };

    /**
        * @brief Get the cofactor matrix of a given matrix
        *
        * @tparam Size the size of the matrix
        * @param matrix the original matrix
        * @return mat<Size> the cofactor matrix generated from the original matrix
        */
    template<length_t Size, typename T>
    mat<Size, Size, T> cofactor(mat<Size, Size, T> const& matrix)
    {
        mat<Size, Size, T> result;

        for (length_t column = 0; column < Size; ++column) 
        {
            for (length_t row = 0; row < Size; ++row) 
            {
                float deter = determinant(minor<Size, T>(matrix, column, row));
                result[column][row] = deter * (1 + (column ^ row) * -2);
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
    //mat1 cofactor(mat1 const& matrix) { return matrix; }

    /**
        * @brief Get the adjugate matrix of a given matrix
        *
        * @tparam Size the size of the original matrix
        * @param matrix the original matrix
        * @return mat<Size> the adjugate matrix gernerated from the original matrix
        */
    template<length_t Size, typename T>
    mat<Size, Size, T> adjugate(mat<Size, Size, T> const& matrix)
    {
        return transposed(cofactor(matrix));
    }

    /**
        * @brief Get the inverse matrix of given matrix
        *
        * @tparam Size the size of the original matrix
        * @param matrix the matrix that will be used to find its inverse
        * @return an optional result depending on whether an inverse exist.
        * mat<Size> the inverse of the given square matrix
        */
    template<length_t Size, typename T>
    std::optional<mat<Size, Size, T>> inverse(mat<Size, Size, T> const& matrix)
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
    template<>
    std::optional<mat4> inverse(mat4 const& matrix);

    /**
        * @brief boolean to test if an inverse exist
        *
        * @tparam Size the size of the matrix
        * @param matrix the matrix that will be used to test
        * @return true if the determinant of the matrix is not 0
        * @return false if the determinant of the matrix is 0
        */
    template<length_t Size, typename T>
    bool has_inverse(mat<Size, Size, T> const& matrix)
    {
        return !equals(determinant(matrix), 0.0f);
    }

    template<length_t Col, length_t Row, typename T>
    mat<Row, Col, T> transposed(mat<Col, Row, T> const& matrix)
    {
        mat<Row, Col, T> result{};

        for (length_t i = 0; i < Col; ++i)
        {
            for (length_t j = 0; j < Row; ++j)
            {
                result[j][i] = matrix[i][j];
            }
        }

        return result;
    }

    template<length_t Square, typename T>
    mat<Square, Square, T> transpose(mat<Square, Square, T> & matrix)
    {
        matrix = transposed(matrix);
        return matrix;
    }
}