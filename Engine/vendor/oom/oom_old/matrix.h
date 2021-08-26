#pragma once

#include "fwd.h"
#include "vec.h"
#include "maths_utils.h"

#include <array>

namespace oom
{
    template<std::size_t Columns, std::size_t Rows, typename T>
    class matrix
    {
    public:
        using value_type        = T;
        using const_value       = value_type const;
        using reference         = value_type &;
        using const_reference   = value_type const&;

        using pointer           = value_type*;
        using size_type         = std::size_t;

        using container         = std::array<value_type, Columns* Rows>;

        static constexpr bool is_square_matrix = (Columns == Rows);
        static constexpr size_type num_elements = Columns * Rows;
        static constexpr size_type num_rows = Rows;
        static constexpr size_type num_columns = Columns;

        template<size_type size>
        static constexpr bool is_homogeneous_convertible = is_square_matrix &&
            (size == Rows - 1) && (size == Columns - 1);
    
    private:
        
        union
        {
            container values = { value_type {0} };
            
            vec<Columns, value_type> cols[Rows];
        };

    public:


        /**
         * Returns a zero matrix.
         * @return A zero matrix.
         */
        static constexpr matrix zero() { return matrix{}; }

        /**
         * Returns an identity matrix.
         * @return An identity matrix.
         * @warning This function is only defined for square matrices.
         */
        template<size_type Size = Columns>
        static constexpr std::enable_if_t<is_square_matrix, mat<Size>> identity()
        {
            return mat<Size>{ value_type{ 1 } };
        }


        matrix() = default;
        ~matrix() = default;
        matrix(matrix const& copy) = default;
        matrix(matrix && move) = default;
        matrix& operator=(matrix const& copy) = default;
        matrix& operator=(matrix && move) = default;
        
        explicit matrix(container container) : values{ container } {}

        ////explicit matrix(vec<Columns, value_type> vectors[Rows]) : cols{ vectors } {}
        //template<typename S = vec<Columns, value_type>)
        //matrix(vec<Columns, value_type> vectors[Rows]) : cols{ vectors } {}

        template<typename = std::enable_if_t<is_square_matrix>>
        explicit matrix(value_type val) 
        {
            for (size_type i = 0; i < Rows; ++i)
            {
                cols[i][i] = val;
                //values[i * Rows + i] = val;
            }
        }
        
        template<std::size_t size,
            typename = std::enable_if_t<is_homogeneous_convertible<size>>>
        matrix(mat<size> sqrMat)
        {
            set_to_identity();

            for (size_type j = 0; j < Columns - 1; ++j)
            {
                for (size_type i = 0; i < Rows - 1; ++i)
                {
                    cols[i][j] = sqrMat[i][j];
                    //values[j * Rows + i] = sqrMat[i][j];
                }
            }
        }

        //note: copies over in row-major order
        template <class ... Args,
            typename = std::enable_if_t<(std::is_arithmetic_v<std::decay_t<Args>>&& ...) && (sizeof...(Args) == num_elements)>>
        matrix(Args&&...args) : values { std::forward<value_type>(static_cast<value_type>(args))... } {}

        /*template <class ... Args, typename = std::enable_if_t<(std::is_same_v<value_type, std::decay_t<Args>>&& ...) && (sizeof...(Args) == num_elements)>>
        matrix(Args&&...args) : values{ std::forward<Args>(args)... } {}*/

        template <class ... Args, typename V = vec<Columns,value_type>,
            typename = std::enable_if_t<(std::is_same_v<vec<Columns, value_type>, std::decay_t<Args>>&& ...) && (sizeof...(Args) == num_rows)>>
        matrix(Args&&...args) : cols{ std::forward<Args>(args)... } {}

        vec<Columns, value_type> const operator[](size_type column) const { return cols[column]; }

        vec<Columns, value_type>& operator[](size_type column) { return cols[column]; }


        matrix operator-() const
        {
            matrix res{};
            for (size_type j = 0; j < Columns; ++j)
            {
                for (size_type i = 0; i < Rows; ++i)
                {
                    res[i][j] = -cols[i][j];
                }
            }
            return res;
        }
        

        bool operator==(matrix const& rhs) const
        {
            for (size_type i = 0; i < num_elements; i++) {
                if (!approx_equal(values[i], rhs.values[i])) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(matrix const& rhs) const { return !(*this == rhs); }

        /*matrix operator+(const_value scalar) const
        {
            matrix result{ *this };
            for (size_type i = 0; i < num_elements; ++i) {
                result[i] += scalar;
            }
            return result;
        }*/

        matrix operator+(matrix const& rhs) const
        {
            matrix result;
            for (size_type i = 0; i < num_elements; ++i) {
                result.values[i] = values[i] + rhs.values[i];
            }
            return result;
        }

        matrix& operator+=(matrix const& rhs)
        {
            *this = (*this) + rhs;
            return *this;
        }
        
        matrix& operator+=(const_value scalar)
        {
            for (size_type i = 0; i < num_elements; ++i) {
                values[i] += scalar;
            }
            return *this;
        }

        matrix operator-(matrix const& rhs) const
        {
            matrix result;
            for (size_type i = 0; i < num_elements; ++i) {
                result.values[i] = values[i] - rhs.values[i];
            }
            return result;
        }

        matrix& operator-=(matrix const& rhs)
        {
            *this = (*this) - rhs;
            return *this;
        }

        matrix& operator-=(const_value rhs)
        {
            *this = (*this) - rhs;
            return *this;
        }

        matrix operator*(const_value scalar) const
        {
            matrix result;
            for (size_type i = 0; i < num_elements; ++i) {
                result.values[i] = values[i] * scalar;
            }
            return result;
        }

        matrix& operator*=(const_value scalar)
        {
            *this = (*this) * scalar;
            return *this;
        }

        template<size_type VecSize>
        std::enable_if_t<VecSize == Rows, matrix<Columns, 1>> operator*(vec<VecSize> const& rhs)
        {
            matrix<Columns, 1> result;
            matrix<Rows, Columns> trans{ transposed() };

            for (size_type i = 0; i < Columns; ++i)
            {
                result[i][0] = trans.cols[i].dot(rhs);
            }
        }

        template<size_type VecSize>
        std::enable_if_t<VecSize == Rows - 1, matrix<Columns, 1>> operator*(point<VecSize> const& rhs)
        {
            point<Rows> converted{ rhs, 1 };
            matrix<Columns, 1> result;
            matrix<Rows, Columns> trans{ transposed() };

            for (size_type i = 0; i < Columns; ++i)
            {
                result[i][0] = trans.cols[i].dot(converted);
            }
        }

        template<size_type RHSColumns>
        matrix<RHSColumns, Rows> operator*(const matrix<RHSColumns, Columns>& rhs) const
        {
            matrix<RHSColumns, Rows> result;
            for (size_type i = 0; i < RHSColumns; ++i) {
                for (size_type j = 0; j < Rows; ++j) {
                    for (size_type k = 0; k < Columns; ++k) {
                        result[i][j] += (*this)[k][j] * rhs[i][k];
                    }
                }
            }
            return result;
        }

        template<size_type Size = Columns>
        std::enable_if_t <is_square_matrix, matrix&> operator*=(matrix const& rhs)
        {
            *this = (*this) * rhs;
            return *this;
        }

        /**
        * @brief Make this matrix a zero matrix.
        */
        void set_to_zero() { std::memset(values.data(), 0, sizeof(values)); }

        /**
         * @brief Make this matrix an identity matrix.
         * @warning This function is only defined for square matrices.
         */
        template<size_type Size = Columns>
        std::enable_if_t <is_square_matrix> set_to_identity() { (*this) = identity(); }

        /**
        * @brief Transpose this matrix.
        */
        template<size_type Size = Columns>
        std::enable_if_t<is_square_matrix, mat<Size>> transpose() { (*this) = transposed(); return *this; }

        /**
        * @brief Returns the transpose of this matrix.
        * @return The transpose of this matrix.
        */
        matrix<Rows, Columns> transposed()
        {
            matrix<Rows, Columns> result;
            for (size_type i = 0; i < Columns; ++i) {
                for (size_type j = 0; j < Rows; ++j) {
                    result[j][i] = (*this)[i][j];
                }
            }
            return result;
        }

        /**
         * @brief Returns a string representation of this matrix.
         *
         * @param _precision The number of decimals to show for each element
         * @param _padding The padding for each element so that it is printed aligned
         * @return A string representation of this matrix.
         */
        /*std::string to_string(const int _precision = 1, const int _padding = 5) const
        {
            std::ostringstream out;
            out.precision(_precision);
            out << std::fixed;

            for (size_type i = 0; i < Rows; ++i) {
                for (size_type j = 0; j < Columns; ++j) {
                    out
                        << std::setw(_precision + _padding)
                        << (*this)[j][i]
                        << ",";
                }
                out << '\n';
            }

            return out.str();
        }

        friend std::ostream& operator<<(std::ostream& stream, const matrix<Columns, Rows>& mat)
        {
            stream << "["
                << std::to_string(Columns)
                << "X" << std::to_string(Rows) << "]"
                << "matrix:\n" << mat.to_string();
            return stream;
        }*/

        vec<Columns, value_type> data()
        {
            return cols;
        }

    };

    template<std::size_t Columns, std::size_t Rows, typename T>
    matrix<Columns, Rows, T> operator+(typename matrix<Columns, Rows, T>::value_type scalar, matrix<Columns, Rows, T> const& rhs) 
    {
        return rhs + scalar;
    }

    /*template<std::size_t VecSize, std::size_t Rows, std::size_t Columns>
    std::enable_if_t<VecSize == Rows, matrix<Columns, 1>> operator*(matrix<Columns, Rows> matrix, vec<VecSize> const& rhs)
    {
        return rhs * matrix;
    }*/

}