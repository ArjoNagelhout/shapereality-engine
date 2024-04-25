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
            // copy transposed
            std::copy_n(data.begin(), data.size(), data_.begin());
            // todo
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
    constexpr MATRIX_TYPE::OtherMatrix<Rows, OtherColumns> MATRIX_TYPE::operator*(OtherMatrix<OtherRows, OtherColumns> const& other) const
    requires (Columns == OtherRows)
    {
        Matrix<Rows, Columns> result{};
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
        // implementation from: https://stackoverflow.com/questions/1556260/convert-quaternion-rotation-to-rotation-matrix

        float const x = rotation.x;
        float const y = rotation.y;
        float const z = rotation.z;
        float const w = rotation.w;

        float const m00 = 1.0f - 2.0f * y * y - 2.0f * z * z;
        float const m01 = 2.0f * x * y - 2.0f * z * w;
        float const m02 = 2.0f * x * z + 2.0f * y * w;

        float const m10 = 2.0f * x * y + 2.0f * z * w;
        float const m11 = 1.0f - 2.0f * x * x - 2.0f * z * z;
        float const m12 = 2.0f * y * z - 2.0f * x * w;

        float const m20 = 2.0f * x * z - 2.0f * y * w;
        float const m21 = 2.0f * y * z + 2.0f * x * w;
        float const m22 = 1.0f - 2.0f * x * x - 2.0f * y * y;

        return {
            m00, m01, m02, 0.0,
            m10, m11, m12, 0.0,
            m20, m21, m22, 0.0,
            0.0, 0.0, 0.0, 1.0
        };
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
        float const tanHalfFovY = tan(fieldOfViewInRadians / 2.0f);

        Matrix<4, 4, Type, Layout> result;
        result(0, 0) = 1.0f / (aspectRatio * tanHalfFovY);
        result(1, 1) = 1.0f / (tanHalfFovY);
        result(2, 2) = zFar / (zFar - zNear);
        result(3, 2) = 1.0f;
        result(2, 3) = -(zFar * zNear) / (zFar - zNear);
        return result;
    }
#undef MATRIX_TEMPLATE
#undef MATRIX_TYPE
}

#endif //SHAPEREALITY_MATRIX_INL
