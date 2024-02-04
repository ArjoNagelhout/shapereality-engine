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
    // forward declarations
    using vector_size_type = unsigned int;
    using matrix_size_type = unsigned int;

    template<vector_size_type Size>
    struct Vector;
    struct Quaternion;

    template<matrix_size_type Rows, matrix_size_type Columns>
    struct Matrix;

    // shorthand forms
    using Vector2 = Vector<2>;
    using Vector3 = Vector<3>;
    using Vector4 = Vector<4>;

    using Matrix2 = Matrix<2, 2>;
    using Matrix3 = Matrix<3, 3>;
    using Matrix4 = Matrix<4, 4>;
}

#endif //SHAPEREALITY_MATH_CONFIG_H
