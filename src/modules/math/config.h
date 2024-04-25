//
// Created by Arjo Nagelhout on 26/12/2023.
//

#ifndef SHAPEREALITY_MATH_CONFIG_H
#define SHAPEREALITY_MATH_CONFIG_H

/**
 * @namespace math
 * @brief simple templated math library with primitives and functions commonly used for games such as Bounds,
 * Quaternion, Matrix, Vector, Ray
 */
namespace math
{
    using DefaultType = float;
    using SizeType = uint32_t;

    template<SizeType Size, typename Type = DefaultType>
    struct Vector;

    template<typename Type = DefaultType>
    struct Quaternion;

    // memory layout for matrix
    enum class MemoryLayout
    {
        ColumnMajor,
        RowMajor
    };

    // default memory layout
    constexpr MemoryLayout defaultMatrixLayout = MemoryLayout::ColumnMajor;

    template<
        SizeType Rows,
        SizeType Columns,
        typename Type = DefaultType,
        MemoryLayout Layout = defaultMatrixLayout>
    struct Matrix;

    constexpr float const vectorEpsilonFloat = 1e-7f;
    constexpr double const vectorEpsilonDouble = 1e-7;
    constexpr float const matrixEpsilonFloat = 1e-5f;
    constexpr double const matrixEpsilonDouble = 1e-7;

    // shorthand forms

    // default type
    using Vector2 = Vector<2>;
    using Vector3 = Vector<3>;
    using Vector4 = Vector<4>;
    using Matrix2 = Matrix<2, 2>;
    using Matrix3 = Matrix<3, 3>;
    using Matrix4 = Matrix<4, 4>;

    // float
    using Vector2f = Vector<2, float>;
    using Vector3f = Vector<3, float>;
    using Vector4f = Vector<4, float>;
    using Matrix2f = Matrix<2, 2, float>;
    using Matrix3f = Matrix<3, 3, float>;
    using Matrix4f = Matrix<4, 4, float>;
    using Quaternionf = Quaternion<float>;

    // double
    using Vector2d = Vector<2, double>;
    using Vector3d = Vector<3, double>;
    using Vector4d = Vector<4, double>;
    using Matrix2d = Matrix<2, 2, double>;
    using Matrix3d = Matrix<3, 3, double>;
    using Matrix4d = Matrix<4, 4, double>;
    using Quaterniond = Quaternion<double>;

    // int
    using Vector2i = Vector<2, int>;
    using Vector3i = Vector<3, int>;
    using Vector4i = Vector<4, int>;
    using Matrix2i = Matrix<2, 2, int>;
    using Matrix3i = Matrix<3, 3, int>;
    using Matrix4i = Matrix<4, 4, int>;

    // unsigned int
    using Vector2u = Vector<2, unsigned int>;
    using Vector3u = Vector<3, unsigned int>;
    using Vector4u = Vector<4, unsigned int>;
    using Matrix2u = Matrix<2, 2, unsigned int>;
    using Matrix3u = Matrix<3, 3, unsigned int>;
    using Matrix4u = Matrix<4, 4, unsigned int>;
}

#endif //SHAPEREALITY_MATH_CONFIG_H
