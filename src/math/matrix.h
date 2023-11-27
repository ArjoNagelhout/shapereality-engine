#ifndef BORED_ENGINE_MATRIX_H
#define BORED_ENGINE_MATRIX_H

#include <array>
#include <sstream>

namespace math
{
	template<unsigned int Rows, unsigned int Columns>
	class Matrix
	{
	public:
		explicit Matrix() = default;
		explicit Matrix(std::array<std::array<float, Columns>, Rows> _data) : data(_data)
		{}

		~Matrix() = default;

		std::array<std::array<float, Columns>, Rows> data{};

		constexpr unsigned int rows()
		{
			return Rows;
		}

		constexpr unsigned int columns()
		{
			return Columns;
		}

		float operator()(int row, int column)
		{
			return data[row][column];
		}

		// https://en.wikipedia.org/wiki/Matrix_multiplication#Definition
		// naive implementation
		//
		// requirement: columns of a == rows of b
		// will return a matrix of (a.rows, b.columns)
		template<
			unsigned int rhsRows,
			unsigned int rhsColumns,
			unsigned int resultRows = Rows,
			unsigned int resultColumns = Columns
		>
		typename std::enable_if<(Columns == rhsRows), Matrix<resultRows, resultColumns>>::type
		operator*(Matrix<rhsRows, rhsColumns> const& rhs)
		{
			Matrix<resultRows, resultColumns> result{};

			unsigned int const n = Rows;
			unsigned int const m = Columns;
			unsigned int const p = rhsColumns;

			for (int i = 0; i < n; i++)
			{
				for (int j = 0; j < p; j++)
				{
					for (int k = 0; k < m; k++)
					{
						result.data[i][j] += data[i][k] * rhs.data[k][j];
					}
				}
			}

			return result;
		}

		[[nodiscard]] std::string toString()
		{
			std::stringstream result{"{"};
			for (int i = 0; i < Rows; i++)
			{
				if (i > 0)
				{
					result << " ";
				}
				for (int j = 0; j < Columns; j++)
				{
					result << data[i][j];
					if (j < Columns - 1)
					{
						result << ", ";
					}
				}
				if (i < Rows - 1)
				{
					result << ", \n";
				}
			}
			result << "}";
			return result.str();
		}
	};

	using Matrix4x4 = Matrix<4, 4>;
	using Matrix4 = Matrix4x4;
	using mat4 = Matrix4x4;
}

#endif //BORED_ENGINE_MATRIX_H
