#pragma once

#include "matrix/mat1x1.hpp"
//#include "matrix/mat1x2.hpp"
//#include "matrix/mat1x3.hpp"
//#include "matrix/mat1x4.hpp"
//#include "matrix/mat2x1.hpp"
#include "matrix/mat2x2.hpp"
#include "matrix/mat2x3.hpp"
#include "matrix/mat2x4.hpp"
//#include "matrix/mat3x1.hpp"
#include "matrix/mat3x2.hpp"
#include "matrix/mat3x3.hpp"
#include "matrix/mat3x4.hpp"
//#include "matrix/mat4x1.hpp"
#include "matrix/mat4x2.hpp"
#include "matrix/mat4x3.hpp"
#include "matrix/mat4x4.hpp"

namespace oom
{
    // Ensure doesnt compile if attempting to create a matrix with 0 rows or columns of any type
    template<size_t Columns, typename T>
    struct mat<Columns, 0, T>;
    template<size_t Rows, typename T>
    struct mat<0, Rows, T>;

    using mat1   = mat<1, 1, float>;
    using mat2   = mat<2, 2, float>;
    using mat3   = mat<3, 3, float>;
    using mat4   = mat<4, 4, float>;

    //using mat1x2 = mat<1, 2, float>;
    //using mat1x3 = mat<1, 3, float>;
    //using mat1x4 = mat<1, 4, float>;
    using mat2x1 = mat<2, 1, float>;
    using mat2x3 = mat<2, 3, float>;
    using mat2x4 = mat<2, 4, float>;
    using mat3x1 = mat<3, 1, float>;
    using mat3x2 = mat<3, 2, float>;
    using mat3x4 = mat<3, 4, float>;
    using mat4x1 = mat<4, 1, float>;
    using mat4x2 = mat<4, 2, float>;
    using mat4x3 = mat<4, 3, float>;

    //using dmat1   = mat<1, 1, double>;
    using dmat2   = mat<2, 2, double>;
    using dmat3   = mat<3, 3, double>;
    using dmat4   = mat<4, 4, double>;
    //using dmat1x2 = mat<1, 2, double>;
    //using dmat1x3 = mat<1, 3, double>;
    //using dmat1x4 = mat<1, 4, double>;
    using dmat2x1 = mat<2, 1, double>;
    using dmat2x3 = mat<2, 3, double>;
    using dmat2x4 = mat<2, 4, double>;
    using dmat3x1 = mat<3, 1, double>;
    using dmat3x2 = mat<3, 2, double>;
    using dmat3x4 = mat<3, 4, double>;
    using dmat4x1 = mat<4, 1, double>;
    using dmat4x2 = mat<4, 2, double>;
    using dmat4x3 = mat<4, 3, double>;
    
    //using imat1   = mat<1, 1, int>;
    using imat2   = mat<2, 2, int>;
    using imat3   = mat<3, 3, int>;
    using imat4   = mat<4, 4, int>;
    //using imat1x2 = mat<1, 2, int>;
    //using imat1x3 = mat<1, 3, int>;
    //using imat1x4 = mat<1, 4, int>;
    using imat2x1 = mat<2, 1, int>;
    using imat2x3 = mat<2, 3, int>;
    using imat2x4 = mat<2, 4, int>;
    using imat3x1 = mat<3, 1, int>;
    using imat3x2 = mat<3, 2, int>;
    using imat3x4 = mat<3, 4, int>;
    using imat4x1 = mat<4, 1, int>;
    using imat4x2 = mat<4, 2, int>;
    using imat4x3 = mat<4, 3, int>;

    //using umat1   = mat<1, 1, unsigned int>;
    using umat2   = mat<2, 2, unsigned int>;
    using umat3   = mat<3, 3, unsigned int>;
    using umat4   = mat<4, 4, unsigned int>;
    //using umat1x2 = mat<1, 2, unsigned int>;
    //using umat1x3 = mat<1, 3, unsigned int>;
    //using umat1x4 = mat<1, 4, unsigned int>;
    using umat2x1 = mat<2, 1, unsigned int>;
    using umat2x3 = mat<2, 3, unsigned int>;
    using umat2x4 = mat<2, 4, unsigned int>;
    using umat3x1 = mat<3, 1, unsigned int>;
    using umat3x2 = mat<3, 2, unsigned int>;
    using umat3x4 = mat<3, 4, unsigned int>;
    using umat4x1 = mat<4, 1, unsigned int>;
    using umat4x2 = mat<4, 2, unsigned int>;
    using umat4x3 = mat<4, 3, unsigned int>;
}
