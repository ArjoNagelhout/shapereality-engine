#ifndef SHAPEREALITY_MATRIX_H
#define SHAPEREALITY_MATRIX_H

#include <array>
#include <sstream>
#include <functional>

#include "config.h"

namespace math
{
    /**
     * @tparam Layout whether the data internally is stored using column-major or row-major layout
     * @tparam Rows
     * @tparam Columns
     */
    template<SizeType Rows, SizeType Columns, typename Type, MemoryLayout Layout>
    struct Matrix final
    {
        // matrix operations are only valid for types that have the same Type, Interface and Layout template arguments
        template<SizeType Rows_, SizeType Columns_> using OtherMatrix = Matrix<Rows_, Columns_, Type, Layout>;

        //--------------------------------
        // Construct, copy, move, destruct
        //--------------------------------

        // construct with all zeros
        constexpr explicit Matrix();

        // construct from initializer list with provided memory layout, default is row major
        constexpr Matrix(std::initializer_list<Type> data, MemoryLayout dataLayout = MemoryLayout::RowMajor);

        // move constructor
        constexpr Matrix(Matrix&& other) noexcept;

        // move assignment operator
        constexpr Matrix& operator=(Matrix&& other) noexcept;

        // copy constructor
        constexpr Matrix(Matrix const& other);

        // copy assignment operator
        constexpr Matrix& operator=(Matrix const& other);

        constexpr ~Matrix();

        //-----------
        // Properties
        //-----------

        // get rows * columns
        [[nodiscard]] constexpr static SizeType size();

        // get amount of columns
        [[nodiscard]] constexpr static SizeType columns();

        // get amount of rows
        [[nodiscard]] constexpr static SizeType rows();

        //-------
        // Access
        //-------

        // get reference to component at (row, column)
        [[nodiscard]] constexpr Type& operator()(SizeType row, SizeType column);

        // get const reference to component at (row, column)
        [[nodiscard]] constexpr Type const& operator()(SizeType row, SizeType column) const;

        // get value of component at (row, column)
        [[nodiscard]] constexpr Type get(SizeType row, SizeType column) const;

        // set value of component at (row, column)
        constexpr void set(SizeType row, SizeType column, Type value);

        //---------
        // Equality
        //---------

        // get whether this matrix is exactly equal to the given matrix
        [[nodiscard]] constexpr bool operator==(Matrix const& other) const;

        // get whether this matrix is not exactly equal to the given matrix
        [[nodiscard]] constexpr bool operator!=(Matrix const& other) const;

        // approximate equality using epsilon
        [[nodiscard]] constexpr bool approximatelyEquals(Matrix const& other)
        requires (std::is_same_v<Type, float> || std::is_same_v<Type, double>);

        //----------
        // Operators
        //----------

        // component-wise add a matrix to this matrix and return the result as a copy
        [[nodiscard]] constexpr Matrix operator+(Matrix const& other) const;

        // component-wise subtract a matrix from this matrix and return the result as a copy
        [[nodiscard]] constexpr Matrix operator-(Matrix const& other) const;

        // multiply this matrix by a scalar and return a copy
        [[nodiscard]] constexpr Matrix operator*(Type scalar) const;

        // divide this matrix by a scalar and return a copy
        [[nodiscard]] constexpr Matrix operator/(Type scalar) const;

        // component-wise add a matrix to this matrix in place
        constexpr void operator+=(Matrix const& other);

        // component-wise subtract a matrix from this matrix in place
        constexpr void operator-=(Matrix const& other);

        // multiply a matrix by a scalar in place
        constexpr void operator*=(Type scalar);

        // divide a matrix by a scalar in place
        constexpr void operator/=(Type scalar);

        //----------------------
        // Matrix multiplication
        //----------------------

        /**
         * matrix multiplication with another matrix
         *
         * @param other
         * @return returns a matrix of size (this.rows, other.columns)
         * @note column count of `this` should be equal to row count of `other`
         */
        template<SizeType OtherRows, SizeType OtherColumns>
        constexpr OtherMatrix<OtherRows, OtherColumns> operator*(OtherMatrix<OtherRows, OtherColumns> const& other) const
        requires (Columns == OtherRows);

        //-----------------
        // Member functions
        //-----------------

        // transpose this matrix in place, only valid if Rows equals Columns
        constexpr void transpose()
        requires (Rows == Columns);

        // get a transposed copy of this matrix
        [[nodiscard]] constexpr OtherMatrix<Columns, Rows> getTranspose() const;

        // get an inverse copy of this matrix
        // note: calculating the inverse is not trivial, so is only implemented as specializations for
        // Matrix<2, 2>, Matrix<3, 3> and Matrix<4, 4>
        [[nodiscard]] constexpr Matrix getInverse() const;

        //-------
        // Static
        //-------

        // create identity matrix
        [[nodiscard]] constexpr static Matrix createIdentity()
        requires (Rows == Columns);

        const static Matrix identity;
        const static Matrix zero;

    private:
        std::array<float, Columns * Rows> data_{};

        // get reference to component at (row, column)
        constexpr Type const& getImplementation(SizeType row, SizeType column) const;

        // iterate
        template<typename Function>
        constexpr void forEach(Function&& function);

        template<typename Function>
        constexpr void forEach(Function&& function) const;
    };

    // multiply matrix by scalar
    template<SizeType Rows, SizeType Columns, typename Type = DefaultType, MemoryLayout Layout = defaultMatrixLayout>
    [[nodiscard]] constexpr Matrix<Rows, Columns, Type, Layout> operator*(float lhs, Matrix<Rows, Columns, Type, Layout> const& rhs);

    // create translation matrix
    template<typename Type = DefaultType, MemoryLayout Layout = defaultMatrixLayout>
    [[nodiscard]] constexpr Matrix<4, 4, Type, Layout> createTranslationMatrix(Vector<3, Type> const& translation);

    // create rotation matrix
    template<typename Type = DefaultType, MemoryLayout Layout = defaultMatrixLayout>
    [[nodiscard]] constexpr Matrix<4, 4, Type, Layout> createRotationMatrix(Quaternion<Type> const& rotation);

    // create scale matrix
    template<typename Type = DefaultType, MemoryLayout Layout = defaultMatrixLayout>
    [[nodiscard]] constexpr Matrix<4, 4, Type, Layout> createScaleMatrix(Vector<3, Type> const& scale);

    // create a translation, rotation, scale matrix
    template<typename Type = DefaultType, MemoryLayout Layout = defaultMatrixLayout>
    [[nodiscard]] constexpr Matrix<4, 4, Type, Layout> createTRSMatrix(
        Vector<3, Type> const& translation, Quaternion<Type> const& rotation, Vector<3, Type> const& scale);

    // creates a perspective projection matrix.
    template<typename Type = DefaultType, MemoryLayout Layout = defaultMatrixLayout>
    [[nodiscard]] constexpr Matrix<4, 4, Type, Layout> createPerspectiveProjectionMatrix(
        Type fieldOfViewInRadians, Type aspectRatio, Type zNear, Type zFar);
}

#endif //SHAPEREALITY_MATRIX_H
