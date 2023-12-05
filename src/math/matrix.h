#ifndef BORED_ENGINE_MATRIX_H
#define BORED_ENGINE_MATRIX_H

#include <array>
#include <sstream>

namespace math
{
	template<unsigned int Rows, unsigned int Columns>
	struct Matrix final
	{
		// construct matrix with all zeroes
		explicit Matrix() : _data({})
		{
		}

		// construct matrix with provided
		explicit Matrix(std::array<std::array<float, Columns>, Rows> data) : _data(data)
		{
		}

		~Matrix() = default;

		//
		constexpr unsigned int rows()
		{
			return Rows;
		}

		//
		constexpr unsigned int columns()
		{
			return Columns;
		}

		// get value of this matrix at given row and column index
		constexpr float& operator()(unsigned int row, unsigned int column)
		{
			static_assert(row < Rows && column < Columns);
			return _data[row][column];
		}

		constexpr float get(unsigned int row, unsigned int column) const;

		constexpr void set(unsigned int row, unsigned int column);

		// matrix multiplication between two matrices
		// returns a matrix of size (lhs.rows, rhs.columns)
		// note: column count of `lhs` should be equal to row count of `rhs`
		template<unsigned int rhsRows, unsigned int rhsColumns>
		constexpr Matrix<Rows, rhsColumns> operator*(Matrix<rhsRows, rhsColumns> const& rhs) requires (Columns == rhsRows);

		//
		[[nodiscard]] std::string toString() const;

		//
		constexpr static Matrix identity() requires (Rows == Columns);

		constexpr static Matrix zero();

	private:
		std::array<std::array<float, Columns>, Rows> _data{};
	};

	template<unsigned int Rows, unsigned int Columns>
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
