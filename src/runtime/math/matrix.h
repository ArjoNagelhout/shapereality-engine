#ifndef SHAPEREALITY_MATRIX_H
#define SHAPEREALITY_MATRIX_H

#include <array>
#include <sstream>

#include "config.h"

namespace math
{
    template<matrix_size_type Rows, matrix_size_type Columns>
    struct Matrix final
    {
        // construct matrix with all zeroes
        constexpr explicit Matrix() = default;

        // construct matrix with array of array
        constexpr explicit Matrix(std::array<std::array<float, Columns>, Rows> data) : _data(data)
        {}

        // construct matrix from array of vectors
        constexpr explicit Matrix(std::array<Vector<Columns>, Rows> data)
        {
            for (matrix_size_type row = 0; row < Rows; row++)
            {
                for (matrix_size_type column = 0; column < Columns; column++)
                {
                    operator()(row, column) = data[row].get(column);
                }
            }
        }

        constexpr ~Matrix() = default;

        //
        [[nodiscard]] constexpr matrix_size_type rows() const
        {
            return Rows;
        }

        //
        [[nodiscard]] constexpr matrix_size_type columns()
        {
            return Columns;
        }

        // access a component of this vector at the given row and column index
        // note: as this returns a reference, the value can be altered.
        // If this is undesirable (e.g. to keep a function const) use `get()`
        constexpr float& operator()(matrix_size_type row, matrix_size_type column);

        //
        [[nodiscard]] constexpr float get(matrix_size_type row, matrix_size_type column) const;

        // get the vector of a given row, returns a vector of length Columns
        [[nodiscard]] constexpr Vector<Columns> getRow(matrix_size_type row) const;

        // get the vector of a given row, returns a vector of length Columns
        [[nodiscard]] constexpr Vector<Rows> getColumn(matrix_size_type column) const;

        //
        constexpr void set(matrix_size_type row, matrix_size_type column, float value);

        // returns a matrix that is this matrix flipped along its diagonal
        // see: https://en.wikipedia.org/wiki/Transpose
        constexpr Matrix<Columns, Rows> transpose() const;

        // return the inverse of this matrix
        [[nodiscard]] constexpr Matrix inverse() const;

        // epsilon for `roughlyEquals`
        constexpr static const float kEpsilon = 1e-5f;

        // get whether this matrix is roughly equal to a given matrix
        // uses `epsilon` to mitigate floating point imprecision
        [[nodiscard]] constexpr static bool roughlyEquals(Matrix const& lhs, Matrix const& rhs, float epsilon = kEpsilon);

        // multiply this matrix by a scalar and return a copy
        constexpr Matrix operator*(float rhs) const;

        // add a matrix to this matrix component-wise and return the result as a copy
        constexpr Matrix operator+(Matrix const& rhs) const;

        // subtract a matrix from this matrix component-wise and return the result as a copy
        constexpr Matrix operator-(Matrix const& rhs) const;

        // add a matrix to this matrix component-wise in place
        constexpr void operator+=(Matrix const& rhs);

        // subtract a matrix from this matrix component-wise in place
        constexpr void operator-=(Matrix const& rhs);

        // multiply a matrix by a scalar in place
        constexpr void operator*=(float rhs);

        // get whether this matrix is exactly equal to a given matrix
        // (use `roughlyEquals` to avoid floating point precision problems)
        constexpr bool operator==(Matrix const& rhs) const;

        // get whether this matrix is not exactly equal to a given matrix
        // (use `roughlyEquals` to avoid floating point precision problems)
        constexpr bool operator!=(Matrix const& rhs) const;

        // matrix multiplication between two matrices
        // returns a matrix of size (lhs.rows, rhs.columns)
        // note: column count of `lhs` should be equal to row count of `rhs`
        template<matrix_size_type rhsRows, matrix_size_type rhsColumns>
        constexpr Matrix<Rows, rhsColumns> operator*(Matrix<rhsRows, rhsColumns> const& rhs) const
        requires (Columns == rhsRows);

        // get the formatted string representation of this matrix
        [[nodiscard]] std::string toString() const;

        // creates an identity matrix where each component is 0, except diagonally from [0, 0] to [Rows, Columns]. Those components are set to 1
        // for example:
        // [1, 0, 0]
        // [0, 1, 0]
        // [0, 0, 1]
        constexpr static Matrix createIdentity()
        requires (Rows == Columns);

        const static Matrix identity;
        const static Matrix zero;

    private:
        std::array<std::array<float, Columns>, Rows> _data{};
    };

    template<matrix_size_type Rows, matrix_size_type Columns>
    constexpr std::ostream& operator<<(std::ostream& ostream, Matrix<Rows, Columns> const& matrix);

    template<matrix_size_type Rows, matrix_size_type Columns>
    constexpr Matrix<Rows, Columns> operator*(float lhs, Matrix<Rows, Columns> const& rhs);

    // create a translation matrix
    // note: these methods are placed outside the Matrix class as it would require template specialization and inheritance
    // and this is way simpler.
    constexpr static Matrix<4, 4> createTranslationMatrix(Vector<3> translation);

    // create a rotation matrix
    constexpr static Matrix<4, 4> createRotationMatrix(Quaternion rotation);

    // create a scale matrix
    constexpr static Matrix<4, 4> createScaleMatrix(Vector<3> scale);

    // create a translation, rotation, scale matrix
    constexpr static Matrix<4, 4>
    createTranslationRotationScaleMatrix(Vector<3> translation, Quaternion rotation, Vector<3> scale);

    // creates a perspective projection matrix
    // WARNING: fieldOfView should be in radians!
    constexpr static Matrix<4, 4>
    createPerspectiveProjectionMatrix(float fieldOfViewInRadians, float aspectRatio, float zNear, float zFar);

    // creates an orthographic projection matrix
    constexpr static Matrix<4, 4>
    createOrthographicProjectionMatrix(float left, float right, float top, float bottom, float zNear, float zFar);

    // creates a transformation view matrix that looks at a given point from a given point
    // set up to for example Vector<3>{{0, -1, 0}} to make the view upside down
    constexpr static Matrix<4, 4> createLookAtMatrix(Vector<3> eye, Vector<3> target, Vector<3> up);

    // get the translation of a matrix
    constexpr static Vector<3> getMatrixTranslation(Matrix<4, 4> const& matrix);

    // decompose a matrix into its rotation and scale components
    constexpr static void decomposeMatrix(Matrix<4, 4> const& matrix, Quaternion& outRotation, Vector<3> outScale);
}

#endif //SHAPEREALITY_MATRIX_H
