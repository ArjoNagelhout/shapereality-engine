//
// Created by Arjo Nagelhout on 26/12/2023.
//

#ifndef SHAPEREALITY_CONFIG_H
#define SHAPEREALITY_CONFIG_H

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
	using vec2 = Vector<2>;
	using vec3 = Vector<3>;
	using vec4 = Vector<4>;

	using mat2 = Matrix<2, 2>;
	using mat3 = Matrix<3, 3>;
	using mat4 = Matrix<4, 4>;
}

#endif //SHAPEREALITY_CONFIG_H
