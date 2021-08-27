//#pragma once
//
//#include "../vector/vec1.hpp"
//#include <limits>
//#include <cstddef>
//
//namespace oom
//{
//    template<typename T>
//    struct mat<1, 2, T>
//    {
//        using col_type          = vec<2, T>;
//        using row_type          = vec<1, T>;
//        using type              = mat<1, 2, T>;
//        using transpose_type    = mat<2, 1, T>;
//        using value_type        = T;
//
//    private:
//        col_type value;
//
//    public:
//        // -- Accesses --
//
//        using length_type = std::size_t;
//        static constexpr length_type size() { return 1; }
//
//        col_type& operator[](length_type i);
//        constexpr col_type const& operator[](length_type i) const;
//
//        // -- Constructors --
//
//        constexpr mat() = default;
//
//        constexpr mat(mat<1, 2, T> const& m);
//
//        explicit constexpr mat(T scalar);
//        constexpr mat(
//            T const& x,
//            T const& y);
//        constexpr mat(
//            col_type const& v);
//
//        // -- Conversions --
//
//        template<typename U, typename V>
//        constexpr mat(
//            U const& x, V const& y);
//
//        template<typename U, typename V>
//        constexpr mat(
//            vec<2, U> const& v1,
//            );
//
//        // -- Matrix conversions --
//
//        template<typename U>
//        OOM_EXPLICIT constexpr mat(mat<1, 2, U> const& m);
//
//        OOM_EXPLICIT constexpr mat(mat<2, 2, T> const& x);
//        OOM_EXPLICIT constexpr mat(mat<3, 3, T> const& x);
//        OOM_EXPLICIT constexpr mat(mat<4, 4, T> const& x);
//        
//        OOM_EXPLICIT constexpr mat(mat<1, 3, T> const& x);
//
//        OOM_EXPLICIT constexpr mat(mat<3, 1, T> const& x);
//        OOM_EXPLICIT constexpr mat(mat<1, 4, T> const& x);
//        OOM_EXPLICIT constexpr mat(mat<4, 1, T> const& x);
//        OOM_EXPLICIT constexpr mat(mat<2, 3, T> const& x);
//        OOM_EXPLICIT constexpr mat(mat<3, 2, T> const& x);
//        OOM_EXPLICIT constexpr mat(mat<2, 4, T> const& x);
//        OOM_EXPLICIT constexpr mat(mat<4, 2, T> const& x);
//        OOM_EXPLICIT constexpr mat(mat<3, 4, T> const& x);
//        OOM_EXPLICIT constexpr mat(mat<4, 3, T> const& x);
//
//        // -- Unary arithmetic operators --
//
//        template<typename U>
//        mat<1, 1, T>& operator=(mat<1, 1, U> const& m);
//        template<typename U>
//        mat<1, 1, T>& operator+=(U s);
//        template<typename U>
//        mat<1, 1, T>& operator+=(mat<1, 1, U> const& m);
//        template<typename U>
//        mat<1, 1, T>& operator-=(U s);
//        template<typename U>
//        mat<1, 1, T>& operator-=(mat<1, 1, U> const& m);
//        template<typename U>
//        mat<1, 1, T>& operator*=(U s);
//        template<typename U>
//        mat<1, 1, T>& operator*=(mat<1, 1, U> const& m);
//        template<typename U>
//        mat<1, 1, T>& operator/=(U s);
//        template<typename U>
//        mat<1, 1, T>& operator/=(mat<1, 1, U> const& m);
//
//        // -- Increment and decrement operators --
//
//        mat<1, 1, T>& operator++ ();
//        mat<1, 1, T>& operator-- ();
//        mat<1, 1, T> operator++(int);
//        mat<1, 1, T> operator--(int);
//
//        // -- Helper Functions --
//
//        static constexpr mat<1, 1, T> zero() { return mat<1, 1, T>{0}; }
//
//        static constexpr mat<1, 1, T> identity()
//        {
//            return mat<1, 1, T>
//            {
//                { 1 }
//            };
//        };
//
//        void SetToZero() { *this = zero(); }
//        void SetToIdentity() { *this = identity(); }
//        mat<1, 1, T> Transposed() const { return transposed(*this); }
//        mat<1, 1, T> Transpose() { return transpose(*this); }
//
//    };
//
//    // -- Unary operators --
//
//    template<typename T>
//    mat<1, 1, T> operator+(mat<1, 1, T> const& m);
//
//    template<typename T>
//    mat<1, 1, T> operator-(mat<1, 1, T> const& m);
//
//    // -- Binary operators --
//
//    template<typename T>
//    mat<1, 1, T> operator+(mat<1, 1, T> const& m, T scalar);
//
//    template<typename T>
//    mat<1, 1, T> operator+(T scalar, mat<1, 1, T> const& m);
//
//    template<typename T>
//    mat<1, 1, T> operator+(mat<1, 1, T> const& m1, mat<1, 1, T> const& m2);
//
//    template<typename T>
//    mat<1, 1, T> operator-(mat<1, 1, T> const& m, T scalar);
//
//    template<typename T>
//    mat<1, 1, T> operator-(T scalar, mat<1, 1, T> const& m);
//
//    template<typename T>
//    mat<1, 1, T> operator-(mat<1, 1, T> const& m1, mat<1, 1, T> const& m2);
//
//    template<typename T>
//    mat<1, 1, T> operator*(mat<1, 1, T> const& m, T scalar);
//
//    template<typename T>
//    mat<1, 1, T> operator*(T scalar, mat<1, 1, T> const& m);
//
//    template<typename T>
//    typename mat<1, 1, T>::col_type operator*(mat<1, 1, T> const& m, typename mat<1, 1, T>::row_type const& v);
//
//    template<typename T>
//    typename mat<1, 1, T>::row_type operator*(typename mat<1, 1, T>::col_type const& v, mat<1, 1, T> const& m);
//
//    template<typename T>
//    mat<1, 1, T> operator*(mat<1, 1, T> const& m1, mat<1, 1, T> const& m2);
//
//    template<typename T>
//    mat<2, 1, T> operator*(mat<1, 1, T> const& m1, mat<2, 1, T> const& m2);
//
//    template<typename T>
//    mat<3, 1, T> operator*(mat<1, 1, T> const& m1, mat<3, 1, T> const& m2);
//
//    template<typename T>
//    mat<4, 1, T> operator*(mat<1, 1, T> const& m1, mat<4, 1, T> const& m2);
//
//    template<typename T>
//    mat<1, 1, T> operator/(mat<1, 1, T> const& m, T scalar);
//
//    template<typename T>
//    mat<1, 1, T> operator/(T scalar, mat<1, 1, T> const& m);
//
//    template<typename T>
//    typename mat<1, 1, T>::col_type operator/(mat<1, 1, T> const& m, typename mat<1, 1, T>::row_type const& v);
//
//    template<typename T>
//    typename mat<1, 1, T>::row_type operator/(typename mat<1, 1, T>::col_type const& v, mat<1, 1, T> const& m);
//
//    template<typename T>
//    mat<1, 1, T> operator/(mat<1, 1, T> const& m1, mat<1, 1, T> const& m2);
//
//    // -- Boolean operators --
//
//    template<typename T>
//    bool operator==(mat<1, 1, T> const& m1, mat<1, 1, T> const& m2);
//
//    template<typename T>
//    bool operator!=(mat<1, 1, T> const& m1, mat<1, 1, T> const& m2);
//} //namespace oom
//
//#include "mat1x1_impl.hpp"
