//
// Created by Arjo Nagelhout on 01/12/2023.
//

#ifndef SHAPEREALITY_MATRIX_INL
#define SHAPEREALITY_MATRIX_INL

#include <cassert>
#include "matrix.h"

#include "vector.h"
#include "vector.inl"
#include "quaternion.h"

namespace math
{
// Macros are evil, but the definitions for template types with four template parameters
// makes it so unreadable, it is the lesser of two evils.
#if (defined(MATRIX_TEMPLATE) or (defined(MATRIX_TYPE)))
    static_assert(false);
#endif
#define MATRIX_TEMPLATE template<SizeType Rows, SizeType Columns, typename Type, MemoryLayout Layout>
#define MATRIX_TYPE Matrix<Rows, Columns, Type, Layout>

    //--------------------------------
    // Construct, copy, move, destruct
    //--------------------------------

    MATRIX_TEMPLATE
    constexpr MATRIX_TYPE::Matrix() = default;

    MATRIX_TEMPLATE
    constexpr MATRIX_TYPE::Matrix(std::initializer_list<Type> data, MemoryLayout dataLayout)
    {
        if (dataLayout == Layout)
        {
            std::copy_n(data.begin(), data.size(), data_.begin());
        }
        else
        {
            // transpose
            for (SizeType row = 0; row < Rows; row++)
            {
                for (SizeType column = 0; column < Columns; column++)
                {
                    auto input = data.begin();
                    SizeType rowMajorIndex = row * Columns + column;
                    SizeType columnMajorIndex = column * Rows + row;

                    if constexpr (Layout == MemoryLayout::ColumnMajor)
                    {
                        data_[columnMajorIndex] = *(input + rowMajorIndex);
                    }
                    else if constexpr (Layout == MemoryLayout::RowMajor)
                    {
                        data_[rowMajorIndex] = *(input + columnMajorIndex);
                    }
                }
            }
        }
    }

    MATRIX_TEMPLATE
    constexpr MATRIX_TYPE::Matrix(Matrix&& other) noexcept
        : data_(std::move(other.data_))
    {
    }

    MATRIX_TEMPLATE
    constexpr MATRIX_TYPE& MATRIX_TYPE::operator=(Matrix&& other) noexcept
    {
        data_ = std::move(other.data_);
        return *this;
    }

    MATRIX_TEMPLATE
    constexpr MATRIX_TYPE::Matrix(Matrix const& other)
        : data_(other.data_)
    {
    }

    MATRIX_TEMPLATE
    constexpr MATRIX_TYPE& MATRIX_TYPE::operator=(Matrix const& other)
    {
        data_ = other.data_;
        return *this;
    }

    MATRIX_TEMPLATE
    constexpr MATRIX_TYPE::~Matrix() = default;

    //-----------
    // Properties
    //-----------

    MATRIX_TEMPLATE
    constexpr SizeType MATRIX_TYPE::size()
    {
        return Rows * Columns;
    }

    MATRIX_TEMPLATE
    constexpr SizeType MATRIX_TYPE::columns()
    {
        return Columns;
    }

    MATRIX_TEMPLATE
    constexpr SizeType MATRIX_TYPE::rows()
    {
        return Rows;
    }

    //-------
    // Access
    //-------

    MATRIX_TEMPLATE
    constexpr Type& MATRIX_TYPE::operator()(SizeType row, SizeType column)
    {
        return const_cast<Type&>(getImplementation(row, column));
    }

    MATRIX_TEMPLATE
    constexpr Type const& MATRIX_TYPE::operator()(SizeType row, SizeType column) const
    {
        return getImplementation(row, column);
    }

    MATRIX_TEMPLATE
    constexpr Type MATRIX_TYPE::get(SizeType row, SizeType column) const
    {
        return getImplementation(row, column);
    }

    MATRIX_TEMPLATE
    constexpr void MATRIX_TYPE::set(SizeType row, SizeType column, Type value)
    {
        getImplementation(row, column) = value;
    }

    //---------
    // Equality
    //---------

    MATRIX_TEMPLATE
    constexpr bool MATRIX_TYPE::operator==(Matrix const& other) const
    {
        return data_ == other.data_;
    }

    MATRIX_TEMPLATE
    constexpr bool MATRIX_TYPE::operator!=(Matrix const& other) const
    {
        return data_ != other.data_;
    }

    MATRIX_TEMPLATE
    constexpr bool MATRIX_TYPE::approximatelyEquals(Matrix const& other)
    requires (std::is_same_v<Type, float> || std::is_same_v<Type, double>)
    {
        Type epsilon;
        if constexpr (std::is_same_v<Type, float>)
        {
            epsilon = matrixEpsilonFloat;
        }
        else if constexpr (std::is_same_v<Type, double>)
        {
            epsilon = matrixEpsilonDouble;
        }

        for (SizeType i = 0; i < size(); i++)
        {
            if (std::abs(data_[i] - other.data_[i]) > epsilon)
            {
                return false;
            }
        }
        return true;
    }

    //----------
    // Operators
    //----------

    MATRIX_TEMPLATE
    constexpr MATRIX_TYPE MATRIX_TYPE::operator+(Matrix const& other) const
    {
        Matrix result;
        forEach([&](SizeType row, SizeType column) {
            result(row, column) = this->operator()(row, column) + other(row, column);
        });
        return result;
    }

    MATRIX_TEMPLATE
    constexpr MATRIX_TYPE MATRIX_TYPE::operator-(Matrix const& other) const
    {
        Matrix result;
        forEach([&](SizeType row, SizeType column) {
            result(row, column) = this->operator()(row, column) - other(row, column);
        });
        return result;
    }

    MATRIX_TEMPLATE
    constexpr MATRIX_TYPE MATRIX_TYPE::operator*(Type scalar) const
    {
        Matrix result;
        forEach([&](SizeType row, SizeType column) {
            result(row, column) = this->operator()(row, column) * scalar;
        });
        return result;
    }

    MATRIX_TEMPLATE
    constexpr MATRIX_TYPE MATRIX_TYPE::operator/(Type scalar) const
    {
        Matrix result;
        forEach([&](SizeType row, SizeType column) {
            result(row, column) = this->operator()(row, column) / scalar;
        });
        return result;
    }

    MATRIX_TEMPLATE
    constexpr void MATRIX_TYPE::operator+=(Matrix const& other)
    {
        forEach([&](SizeType row, SizeType column) {
            this->operator()(row, column) += other(row, column);
        });
    }

    MATRIX_TEMPLATE
    constexpr void MATRIX_TYPE::operator-=(Matrix const& other)
    {
        forEach([&](SizeType row, SizeType column) {
            this->operator()(row, column) -= other(row, column);
        });
    }

    MATRIX_TEMPLATE
    constexpr void MATRIX_TYPE::operator*=(Type scalar)
    {
        forEach([&](SizeType row, SizeType column) {
            this->operator()(row, column) *= scalar;
        });
    }

    MATRIX_TEMPLATE
    constexpr void MATRIX_TYPE::operator/=(Type scalar)
    {
        forEach([&](SizeType row, SizeType column) {
            this->operator()(row, column) /= scalar;
        });
    }

    //----------------------
    // Matrix multiplication
    //----------------------

    MATRIX_TEMPLATE
    template<SizeType OtherRows, SizeType OtherColumns>
    constexpr MATRIX_TYPE::OtherMatrix<OtherRows, OtherColumns> MATRIX_TYPE::operator*(OtherMatrix<OtherRows, OtherColumns> const& other) const
    requires (Columns == OtherRows)
    {
        OtherMatrix<OtherRows, OtherColumns> result{};
        for (SizeType i = 0; i < Rows; i++)
        {
            for (SizeType j = 0; j < OtherColumns; j++)
            {
                for (SizeType k = 0; k < Columns; k++)
                {
                    result(i, j) += this->operator()(i, k) * other(k, j);
                }
            }
        }
        return result;
    }

    //-----------------
    // Member functions
    //-----------------

    MATRIX_TEMPLATE
    constexpr void MATRIX_TYPE::transpose()
    requires (Rows == Columns)
    {
        SizeType row = 0;
        while (row != Rows)
        {
            for (SizeType column = row + 1; column < Rows; column++)
            {
                std::swap(this->operator()(row, column), this->operator()(column, row));
            }
            row++;
        }
    }

    MATRIX_TEMPLATE
    constexpr MATRIX_TYPE::OtherMatrix<Columns, Rows> MATRIX_TYPE::getTranspose() const
    {
        OtherMatrix<Columns, Rows> result;
        forEach([&](SizeType row, SizeType column) {
            result(column, row) = this->operator()(row, column);
        });
        return result;
    }

    // this enables partial specialization
    template<SizeType Rows, SizeType Columns, typename Type, MemoryLayout Layout>
    struct InverseHelper
    {
        [[nodiscard]] constexpr static Matrix<Rows, Columns, Type, Layout> call(Matrix<Rows, Columns, Type, Layout> const& this_)
        {
            // not implemented
        }
    };

    template<typename Type, MemoryLayout Layout>
    struct InverseHelper<2, 2, Type, Layout>
    {
        [[nodiscard]] constexpr static Matrix<2, 2, Type, Layout> call(Matrix<2, 2, Type, Layout> const& this_)
        {
            float const determinant = this_(0, 0) * this_(1, 1) - this_(0, 1) * this_(1, 0);

            if (determinant == 0)
            {
                return Matrix<2, 2, Type, Layout>{};
            }

            float const oneOverDeterminant = 1.0f / determinant;

            Matrix<2, 2, Type, Layout> result{
                this_(1, 1) * oneOverDeterminant, -this_(0, 1) * oneOverDeterminant,
                -this_(1, 0) * oneOverDeterminant, this_(0, 0) * oneOverDeterminant
            };
            return result;
        }
    };

    template<typename Type, MemoryLayout Layout>
    struct InverseHelper<3, 3, Type, Layout>
    {
        [[nodiscard]] constexpr static Matrix<3, 3, Type, Layout> call(Matrix<3, 3, Type, Layout> const& this_)
        {
            float const determinant =
                this_(0, 0) * (this_(1, 1) * this_(2, 2) - this_(1, 2) * this_(2, 1))
                - this_(0, 1) * (this_(1, 0) * this_(2, 2) - this_(1, 2) * this_(2, 0))
                + this_(0, 2) * (this_(1, 0) * this_(2, 1) - this_(1, 1) * this_(2, 0));

            if (determinant == 0)
            {
                return Matrix<3, 3, Type, Layout>{};
            }

            float const oneOverDeterminant = 1.0f / determinant;

            Matrix<3, 3, Type, Layout> result{};
            result(0, 0) = +(this_(1, 1) * this_(2, 2) - this_(1, 2) * this_(2, 1)) * oneOverDeterminant;
            result(0, 1) = -(this_(0, 1) * this_(2, 2) - this_(0, 2) * this_(2, 1)) * oneOverDeterminant;
            result(0, 2) = +(this_(0, 1) * this_(1, 2) - this_(0, 2) * this_(1, 1)) * oneOverDeterminant;
            result(1, 0) = -(this_(1, 0) * this_(2, 2) - this_(1, 2) * this_(2, 0)) * oneOverDeterminant;
            result(1, 1) = +(this_(0, 0) * this_(2, 2) - this_(0, 2) * this_(2, 0)) * oneOverDeterminant;
            result(1, 2) = -(this_(0, 0) * this_(1, 2) - this_(0, 2) * this_(1, 0)) * oneOverDeterminant;
            result(2, 0) = +(this_(1, 0) * this_(2, 1) - this_(1, 1) * this_(2, 0)) * oneOverDeterminant;
            result(2, 1) = -(this_(0, 0) * this_(2, 1) - this_(0, 1) * this_(2, 0)) * oneOverDeterminant;
            result(2, 2) = +(this_(0, 0) * this_(1, 1) - this_(0, 1) * this_(1, 0)) * oneOverDeterminant;
            return result;
        }
    };

    template<typename Type, MemoryLayout Layout>
    struct InverseHelper<4, 4, Type, Layout>
    {
        [[nodiscard]] constexpr static Matrix<4, 4, Type, Layout> call(Matrix<4, 4, Type, Layout> const& this_)
        {
            float const s0 = this_(0, 0) * this_(1, 1) - this_(1, 0) * this_(0, 1);
            float const s1 = this_(0, 0) * this_(1, 2) - this_(1, 0) * this_(0, 2);
            float const s2 = this_(0, 0) * this_(1, 3) - this_(1, 0) * this_(0, 3);
            float const s3 = this_(0, 1) * this_(1, 2) - this_(1, 1) * this_(0, 2);
            float const s4 = this_(0, 1) * this_(1, 3) - this_(1, 1) * this_(0, 3);
            float const s5 = this_(0, 2) * this_(1, 3) - this_(1, 2) * this_(0, 3);
            float const c5 = this_(2, 2) * this_(3, 3) - this_(3, 2) * this_(2, 3);
            float const c4 = this_(2, 1) * this_(3, 3) - this_(3, 1) * this_(2, 3);
            float const c3 = this_(2, 1) * this_(3, 2) - this_(3, 1) * this_(2, 2);
            float const c2 = this_(2, 0) * this_(3, 3) - this_(3, 0) * this_(2, 3);
            float const c1 = this_(2, 0) * this_(3, 2) - this_(3, 0) * this_(2, 2);
            float const c0 = this_(2, 0) * this_(3, 1) - this_(3, 0) * this_(2, 1);

            float const determinant = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;

            if (determinant == 0)
            {
                return Matrix<4, 4, Type, Layout>{};
            }

            float const oneOverDeterminant = 1.f / determinant;

            Matrix<4, 4, Type, Layout> result{};
            result(0, 0) = (+this_(1, 1) * c5 - this_(1, 2) * c4 + this_(1, 3) * c3) * oneOverDeterminant;
            result(0, 1) = (-this_(0, 1) * c5 + this_(0, 2) * c4 - this_(0, 3) * c3) * oneOverDeterminant;
            result(0, 2) = (+this_(3, 1) * s5 - this_(3, 2) * s4 + this_(3, 3) * s3) * oneOverDeterminant;
            result(0, 3) = (-this_(2, 1) * s5 + this_(2, 2) * s4 - this_(2, 3) * s3) * oneOverDeterminant;
            result(1, 0) = (-this_(1, 0) * c5 + this_(1, 2) * c2 - this_(1, 3) * c1) * oneOverDeterminant;
            result(1, 1) = (+this_(0, 0) * c5 - this_(0, 2) * c2 + this_(0, 3) * c1) * oneOverDeterminant;
            result(1, 2) = (-this_(3, 0) * s5 + this_(3, 2) * s2 - this_(3, 3) * s1) * oneOverDeterminant;
            result(1, 3) = (+this_(2, 0) * s5 - this_(2, 2) * s2 + this_(2, 3) * s1) * oneOverDeterminant;
            result(2, 0) = (+this_(1, 0) * c4 - this_(1, 1) * c2 + this_(1, 3) * c0) * oneOverDeterminant;
            result(2, 1) = (-this_(0, 0) * c4 + this_(0, 1) * c2 - this_(0, 3) * c0) * oneOverDeterminant;
            result(2, 2) = (+this_(3, 0) * s4 - this_(3, 1) * s2 + this_(3, 3) * s0) * oneOverDeterminant;
            result(2, 3) = (-this_(2, 0) * s4 + this_(2, 1) * s2 - this_(2, 3) * s0) * oneOverDeterminant;
            result(3, 0) = (-this_(1, 0) * c3 + this_(1, 1) * c1 - this_(1, 2) * c0) * oneOverDeterminant;
            result(3, 1) = (+this_(0, 0) * c3 - this_(0, 1) * c1 + this_(0, 2) * c0) * oneOverDeterminant;
            result(3, 2) = (-this_(3, 0) * s3 + this_(3, 1) * s1 - this_(3, 2) * s0) * oneOverDeterminant;
            result(3, 3) = (+this_(2, 0) * s3 - this_(2, 1) * s1 + this_(2, 2) * s0) * oneOverDeterminant;
            return result;
        }
    };

    MATRIX_TEMPLATE
    constexpr MATRIX_TYPE MATRIX_TYPE::getInverse() const
    {
        return InverseHelper<Rows, Columns, Type, Layout>::call(*this);
    }

    //-------
    // Static
    //-------

    MATRIX_TEMPLATE
    constexpr MATRIX_TYPE MATRIX_TYPE::createIdentity()
    requires (Rows == Columns)
    {
        Matrix result;
        for (SizeType i = 0; i < Rows; i++)
        {
            result(i, i) = static_cast<Type>(1.0);
        }
        return result;
    }

    MATRIX_TEMPLATE
    constexpr MATRIX_TYPE MATRIX_TYPE::identity = MATRIX_TYPE::createIdentity();

    MATRIX_TEMPLATE
    constexpr MATRIX_TYPE MATRIX_TYPE::zero = MATRIX_TYPE{};

    //---------
    // Private
    //---------

    MATRIX_TEMPLATE
    constexpr Type const& MATRIX_TYPE::getImplementation(SizeType row, SizeType column) const
    {
        if constexpr (Layout == MemoryLayout::RowMajor)
        {
            // row major means we store each row contiguously
            return data_[row * Columns + column];
        }
        else if constexpr (Layout == MemoryLayout::ColumnMajor)
        {
            // column major means we store each column contiguously (so interleaved from a row major perspective)
            return data_[column * Rows + row];
        }
    }

    MATRIX_TEMPLATE
    template<typename Function>
    constexpr void MATRIX_TYPE::forEach(Function&& function)
    {
        for (SizeType row = 0; row < Rows; row++)
        {
            for (SizeType column = 0; column < Columns; column++)
            {
                function(row, column);
            }
        }
    }

    MATRIX_TEMPLATE
    template<typename Function>
    constexpr void MATRIX_TYPE::forEach(Function&& function) const
    {
        for (SizeType row = 0; row < Rows; row++)
        {
            for (SizeType column = 0; column < Columns; column++)
            {
                function(row, column);
            }
        }
    }

    MATRIX_TEMPLATE
    [[nodiscard]] constexpr MATRIX_TYPE operator*(Type lhs, MATRIX_TYPE const& rhs)
    {
        return rhs * lhs;
    }

    template<typename Type, MemoryLayout Layout>
    constexpr Matrix<4, 4, Type, Layout> createTranslationMatrix(Vector<3, Type> const& translation)
    {
        return {
            1, 0, 0, translation.get(0),
            0, 1, 0, translation.get(1),
            0, 0, 1, translation.get(2),
            0, 0, 0, 1
        };
    }

    template<typename Type, MemoryLayout Layout>
    constexpr Matrix<4, 4, Type, Layout> createRotationMatrix(Quaternion<Type> const& rotation)
    {
        // from glm

        Matrix<4, 4, Type, Layout> result;
        Type qxx(rotation.x * rotation.x);
        Type qyy(rotation.y * rotation.y);
        Type qzz(rotation.z * rotation.z);
        Type qxz(rotation.x * rotation.z);
        Type qxy(rotation.x * rotation.y);
        Type qyz(rotation.y * rotation.z);
        Type qwx(rotation.w * rotation.x);
        Type qwy(rotation.w * rotation.y);
        Type qwz(rotation.w * rotation.z);

        result(0, 0) = Type(1) - Type(2) * (qyy +  qzz);
        result(0, 1) = Type(2) * (qxy + qwz);
        result(0, 2) = Type(2) * (qxz - qwy);

        result(1, 0) = Type(2) * (qxy - qwz);
        result(1, 1) = Type(1) - Type(2) * (qxx +  qzz);
        result(1, 2) = Type(2) * (qyz + qwx);

        result(2, 0) = Type(2) * (qxz + qwy);
        result(2, 1) = Type(2) * (qyz - qwx);
        result(2, 2) = Type(1) - Type(2) * (qxx +  qyy);
        result(3, 3) = Type(1);
        return result;
    }

    template<typename Type, MemoryLayout Layout>
    constexpr Matrix<4, 4, Type, Layout> createScaleMatrix(Vector<3, Type> const& scale)
    {
        // implementation from https://www.brainvoyager.com/bv/doc/UsersGuide/CoordsAndTransforms/SpatialTransformationMatrices.html

        return {
            scale.get(0), 0, 0, 0,
            0, scale.get(1), 0, 0,
            0, 0, scale.get(2), 0,
            0, 0, 0, 1
        };
    }

    template<typename Type, MemoryLayout Layout>
    constexpr Matrix<4, 4, Type, Layout> createTRSMatrix(
        Vector<3, Type> const& translation, Quaternion<Type> const& rotation, Vector<3, Type> const& scale)
    {
        Matrix<4, 4, Type, Layout> translationMatrix = createTranslationMatrix(translation);
        Matrix<4, 4, Type, Layout> rotationMatrix = createRotationMatrix(rotation);
        Matrix<4, 4, Type, Layout> scaleMatrix = createScaleMatrix(scale);
        return translationMatrix * rotationMatrix * scaleMatrix;
    }

    template<typename Type, MemoryLayout Layout>
    constexpr Matrix<4, 4, Type, Layout> createPerspectiveProjectionMatrix(Type fieldOfViewInRadians, Type aspectRatio, Type zNear, Type zFar)
    {
        // from GLM, left-handed, zero-to-one
        Type const tanHalfFovy = tan(fieldOfViewInRadians / static_cast<Type>(2));

        Matrix<4, 4, Type, Layout> result;
        result(0, 0) = static_cast<Type>(1) / (aspectRatio * tanHalfFovy);
        result(1, 1) = static_cast<Type>(1) / (tanHalfFovy);
        result(2, 2) = zFar / (zFar - zNear);
        result(3, 2) = 1.0f;
        result(2, 3) = -(zFar * zNear) / (zFar - zNear);
        return result;
    }
#undef MATRIX_TEMPLATE
#undef MATRIX_TYPE
}

#endif //SHAPEREALITY_MATRIX_INL
