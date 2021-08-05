#pragma once

#include <cstddef>

namespace oom
{
    //Forward declarations
    /*struct maths_utils;
    struct matrix_utils;*/

    template<size_t Size, typename T = float>
    struct vec;

    template<size_t Size, typename T = float>
    struct point;

    template<std::size_t Columns, std::size_t Rows, typename T = float>
    class matrix;

    // Ensure doesnt compile if attempting to create a vector with 0 values of any Type
    template<class T>
    struct vec<0, T>;

    // Common vectors definition
    using vec2 = vec<2, float>;
    using vec1 = vec<1, float>;
    using vec3 = vec<3, float>;
    using vec4 = vec<4, float>;

    using dvec1 = vec<1, double>;
    using dvec2 = vec<2, double>;
    using dvec3 = vec<3, double>;
    using dvec4 = vec<4, double>;

    using ivec1 = vec<1, int>;
    using ivec2 = vec<2, int>;
    using ivec3 = vec<3, int>;
    using ivec4 = vec<4, int>;

    using uvec1 = vec<1, unsigned int>;
    using uvec2 = vec<2, unsigned int>;
    using uvec3 = vec<3, unsigned int>;
    using uvec4 = vec<4, unsigned int>;


    // Ensure doesnt compile if attempting to create a point with 0 values of any Type
    template<class T>
    struct point<0, T>;

    // Common points definition
    using point1 = point<1, float>;
    using point2 = point<2, float>;
    using point3 = point<3, float>;
    using point4 = point<4, float>;

    using dpoint1 = point<1, double>;
    using dpoint2 = point<2, double>;
    using dpoint3 = point<3, double>;
    using dpoint4 = point<4, double>;

    using ipoint1 = point<1, int>;
    using ipoint2 = point<2, int>;
    using ipoint3 = point<3, int>;
    using ipoint4 = point<4, int>;

    using upoint1 = point<1, unsigned int>;
    using upoint2 = point<2, unsigned int>;
    using upoint3 = point<3, unsigned int>;
    using upoint4 = point<4, unsigned int>;


    // Ensure doesnt compile if attempting to create a matrix with 0 rows or columns of any type
    template<size_t Columns, typename T>
    class matrix<Columns, 0, T>;
    template<size_t Rows, typename T>
    class matrix<0, Rows, T>;


    // Common matrix definition
    template<size_t SquareSize, typename T = float>
    using mat = matrix<SquareSize, SquareSize, T>;

    using mat1   = matrix<1, 1, float>;
    using mat2   = matrix<2, 2, float>;
    using mat3   = matrix<3, 3, float>;
    using mat4   = matrix<4, 4, float>;

    using mat1x2 = matrix<1, 2, float>;
    using mat1x3 = matrix<1, 3, float>;
    using mat1x4 = matrix<1, 4, float>;
    using mat2x1 = matrix<2, 1, float>;
    using mat2x3 = matrix<2, 3, float>;
    using mat2x4 = matrix<2, 4, float>;
    using mat3x1 = matrix<3, 1, float>;
    using mat3x2 = matrix<3, 2, float>;
    using mat3x4 = matrix<3, 4, float>;
    using mat4x1 = matrix<4, 1, float>;
    using mat4x2 = matrix<4, 2, float>;
    using mat4x3 = matrix<4, 3, float>;

    using dmat1   = matrix<1, 1, double>;
    using dmat2   = matrix<2, 2, double>;
    using dmat3   = matrix<3, 3, double>;
    using dmat4   = matrix<4, 4, double>;
    using dmat1x2 = matrix<1, 2, double>;
    using dmat1x3 = matrix<1, 3, double>;
    using dmat1x4 = matrix<1, 4, double>;
    using dmat2x1 = matrix<2, 1, double>;
    using dmat2x3 = matrix<2, 3, double>;
    using dmat2x4 = matrix<2, 4, double>;
    using dmat3x1 = matrix<3, 1, double>;
    using dmat3x2 = matrix<3, 2, double>;
    using dmat3x4 = matrix<3, 4, double>;
    using dmat4x1 = matrix<4, 1, double>;
    using dmat4x2 = matrix<4, 2, double>;
    using dmat4x3 = matrix<4, 3, double>;
    
    using imat1   = matrix<1, 1, int>;
    using imat2   = matrix<2, 2, int>;
    using imat3   = matrix<3, 3, int>;
    using imat4   = matrix<4, 4, int>;
    using imat1x2 = matrix<1, 2, int>;
    using imat1x3 = matrix<1, 3, int>;
    using imat1x4 = matrix<1, 4, int>;
    using imat2x1 = matrix<2, 1, int>;
    using imat2x3 = matrix<2, 3, int>;
    using imat2x4 = matrix<2, 4, int>;
    using imat3x1 = matrix<3, 1, int>;
    using imat3x2 = matrix<3, 2, int>;
    using imat3x4 = matrix<3, 4, int>;
    using imat4x1 = matrix<4, 1, int>;
    using imat4x2 = matrix<4, 2, int>;
    using imat4x3 = matrix<4, 3, int>;

    using umat1   = matrix<1, 1, unsigned int>;
    using umat2   = matrix<2, 2, unsigned int>;
    using umat3   = matrix<3, 3, unsigned int>;
    using umat4   = matrix<4, 4, unsigned int>;
    using umat1x2 = matrix<1, 2, unsigned int>;
    using umat1x3 = matrix<1, 3, unsigned int>;
    using umat1x4 = matrix<1, 4, unsigned int>;
    using umat2x1 = matrix<2, 1, unsigned int>;
    using umat2x3 = matrix<2, 3, unsigned int>;
    using umat2x4 = matrix<2, 4, unsigned int>;
    using umat3x1 = matrix<3, 1, unsigned int>;
    using umat3x2 = matrix<3, 2, unsigned int>;
    using umat3x4 = matrix<3, 4, unsigned int>;
    using umat4x1 = matrix<4, 1, unsigned int>;
    using umat4x2 = matrix<4, 2, unsigned int>;
    using umat4x3 = matrix<4, 3, unsigned int>;
}