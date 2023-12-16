#ifndef BORED_ENGINE_MATRIX_H
#define BORED_ENGINE_MATRIX_H

#include <array>
#include <sstream>

namespace math
{
	using matrix_size_type = unsigned int;
	
	template<matrix_size_type Rows, matrix_size_type Columns>
	struct Matrix final
	{
		// construct matrix with all zeroes
		constexpr explicit Matrix() : _data({})
		{
		}

		// construct matrix with provided
		constexpr explicit Matrix(std::array<std::array<float, Columns>, Rows> data) : _data(data)
		{
		}

		constexpr ~Matrix() = default;

		//
		constexpr matrix_size_type rows()
		{
			return Rows;
		}

		//
		constexpr matrix_size_type columns()
		{
			return Columns;
		}

		// access a component of this vector at the given row and column index
		// note: as this returns a reference, the value can be altered.
		// If this is undesirable (e.g. to keep a function const) use `get()`
		constexpr float& operator()(matrix_size_type row, matrix_size_type column);

		constexpr float get(matrix_size_type row, matrix_size_type column) const;

		constexpr void set(matrix_size_type row, matrix_size_type column);

		// matrix multiplication between two matrices
		// returns a matrix of size (lhs.rows, rhs.columns)
		// note: column count of `lhs` should be equal to row count of `rhs`
		template<matrix_size_type rhsRows, matrix_size_type rhsColumns>
		constexpr Matrix<Rows, rhsColumns> operator*(Matrix<rhsRows, rhsColumns> const& rhs) requires (Columns == rhsRows);

		// get the formatted string representation of this matrix
		[[nodiscard]] std::string toString() const;

		// creates an identity matrix where each component is 0, except diagonally from [0, 0] to [Rows, Columns]. Those components are set to 1
		// for example:
		// [1, 0, 0]
		// [0, 1, 0]
		// [0, 0, 1]
		constexpr static Matrix createIdentity() requires (Rows == Columns);

		const static Matrix identity;
		const static Matrix zero;

	private:
		std::array<std::array<float, Columns>, Rows> _data{};
	};

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr std::ostream& operator<<(std::ostream& ostream, Matrix<Rows, Columns> const& matrix)
	{
		ostream << matrix.toString();
		return ostream;
	}

	using mat2 = Matrix<2, 2>;
	using mat3 = Matrix<3, 3>;
	using mat4 = Matrix<4, 4>;
}

#endif //BORED_ENGINE_MATRIX_H
