#ifndef BORED_ENGINE_MATRIX_H
#define BORED_ENGINE_MATRIX_H

#include <array>
#include <sstream>

namespace math
{
	template<unsigned int rows, unsigned int columns>
	class Matrix
	{
	public:
		explicit Matrix() = default;
		explicit Matrix(std::array<std::array<float, columns>, rows> _data) : data(_data)
		{}

		~Matrix() = default;

		std::array<std::array<float, columns>, rows> data{};

		unsigned int rowsCount()
		{
			return rows;
		}

		unsigned int columnsCount()
		{
			return columns;
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
			unsigned int rhs_columns,
			unsigned int result_rows = rows,
			unsigned int result_columns = rhs_columns
		>
		typename std::enable_if<(columns == rhsRows), Matrix<result_rows, result_columns>>::type
		operator*(Matrix<rhsRows, rhs_columns> const& rhs)
		{
			Matrix<result_rows, result_columns> result{};

			unsigned int const n = rows;
			unsigned int const m = columns;
			unsigned int const p = rhs_columns;

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
			for (int i = 0; i < rows; i++)
			{
				if (i > 0)
				{
					result << " ";
				}
				for (int j = 0; j < columns; j++)
				{
					result << data[i][j];
					if (j < columns - 1)
					{
						result << ", ";
					}
				}
				if (i < rows - 1)
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
