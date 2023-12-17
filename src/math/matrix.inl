//
// Created by Arjo Nagelhout on 01/12/2023.
//

#ifndef BORED_ENGINE_MATRIX_INL
#define BORED_ENGINE_MATRIX_INL

#include <cassert>
#include "matrix.h"

#include "vector.h"
#include "vector.inl"
#include "quaternion.h"

namespace math
{
	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr matrix_size_type Matrix<Rows, Columns>::rows() const
	{
		return Rows;
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr matrix_size_type Matrix<Rows, Columns>::columns() const
	{
		return Columns;
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr float& Matrix<Rows, Columns>::operator()(matrix_size_type row, matrix_size_type column)
	{
		return _data[row][column];
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr float Matrix<Rows, Columns>::get(matrix_size_type row, matrix_size_type column) const
	{
		return _data[row][column];
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr Vector<Columns> Matrix<Rows, Columns>::getRow(matrix_size_type row) const
	{
		Vector<Columns> result{};
		for (matrix_size_type column = 0; column < Columns; column++)
		{
			result[column] = get(row, column);
		}
		return result;
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr Vector<Rows> Matrix<Rows, Columns>::getColumn(matrix_size_type column) const
	{
		Vector<Rows> result{};
		for (matrix_size_type row = 0; row < Rows; row++)
		{
			result[row] = get(row, column);
		}
		return result;
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr void Matrix<Rows, Columns>::set(matrix_size_type row, matrix_size_type column, float value)
	{
		_data[row][column] = value;
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr Matrix<Columns, Rows> Matrix<Rows, Columns>::transpose() const
	{
		Matrix<Columns, Rows> result{};

		for (matrix_size_type row = 0; row < Rows; row++)
		{
			for (matrix_size_type column = 0; column < Columns; column++)
			{
				// column and row flipped
				result(column, row) = get(row, column);
			}
		}

		return result;
	}

	template<>
	constexpr Matrix<2, 2> Matrix<2, 2>::inverse() const
	{
		float const oneOverDeterminant = 1.0f / ((get(0, 0) * get(1, 1) - get(0, 1) * get(1, 0)));

		Matrix<2, 2> result{{{
								 {{get(1, 1) * oneOverDeterminant, -get(0, 1) * oneOverDeterminant}},
								 {{-get(1, 0) * oneOverDeterminant, get(0, 0) * oneOverDeterminant}}
							 }}};
		return result;
	}

	template<>
	constexpr Matrix<3, 3> Matrix<3, 3>::inverse() const
	{
		float const determinant = get(0, 0) * (get(1, 1) * get(2, 2) - get(1, 2) * get(2, 1))
								  - get(0, 1) * (get(1, 0) * get(2, 2) - get(1, 2) * get(2, 0))
								  + get(0, 2) * (get(1, 0) * get(2, 1) - get(1, 1) * get(2, 0));
		assert(determinant != 0.f);
		float const oneOverDeterminant = 1.0f / determinant;

		Matrix<3, 3> result{};
		result(0, 0) = +(get(1, 1) * get(2, 2) - get(1, 2) * get(2, 1)) * oneOverDeterminant;
		result(0, 1) = -(get(0, 1) * get(2, 2) - get(0, 2) * get(2, 1)) * oneOverDeterminant;
		result(0, 2) = +(get(0, 1) * get(1, 2) - get(0, 2) * get(1, 1)) * oneOverDeterminant;
		result(1, 0) = -(get(1, 0) * get(2, 2) - get(1, 2) * get(2, 0)) * oneOverDeterminant;
		result(1, 1) = +(get(0, 0) * get(2, 2) - get(0, 2) * get(2, 0)) * oneOverDeterminant;
		result(1, 2) = -(get(0, 0) * get(1, 2) - get(0, 2) * get(1, 0)) * oneOverDeterminant;
		result(2, 0) = +(get(1, 0) * get(2, 1) - get(1, 1) * get(2, 0)) * oneOverDeterminant;
		result(2, 1) = -(get(0, 0) * get(2, 1) - get(0, 1) * get(2, 0)) * oneOverDeterminant;
		result(2, 2) = +(get(0, 0) * get(1, 1) - get(0, 1) * get(1, 0)) * oneOverDeterminant;

		return result;
	}

	// https://stackoverflow.com/questions/2624422/efficient-4x4-matrix-inverse-affine-transform/7596981#7596981
	template<>
	constexpr Matrix<4, 4> Matrix<4, 4>::inverse() const
	{
		float const s0 = get(0, 0) * get(1, 1) - get(1, 0) * get(0, 1);
		float const s1 = get(0, 0) * get(1, 2) - get(1, 0) * get(0, 2);
		float const s2 = get(0, 0) * get(1, 3) - get(1, 0) * get(0, 3);
		float const s3 = get(0, 1) * get(1, 2) - get(1, 1) * get(0, 2);
		float const s4 = get(0, 1) * get(1, 3) - get(1, 1) * get(0, 3);
		float const s5 = get(0, 2) * get(1, 3) - get(1, 2) * get(0, 3);
		float const c5 = get(2, 2) * get(3, 3) - get(3, 2) * get(2, 3);
		float const c4 = get(2, 1) * get(3, 3) - get(3, 1) * get(2, 3);
		float const c3 = get(2, 1) * get(3, 2) - get(3, 1) * get(2, 2);
		float const c2 = get(2, 0) * get(3, 3) - get(3, 0) * get(2, 3);
		float const c1 = get(2, 0) * get(3, 2) - get(3, 0) * get(2, 2);
		float const c0 = get(2, 0) * get(3, 1) - get(3, 0) * get(2, 1);

		// Should check for 0 determinant

		float const determinant = s0 * c5 - s1 * c4 + s2 * c3 + s3 * c2 - s4 * c1 + s5 * c0;
		float const oneOverDeterminant = 1.f / determinant;

		Matrix<4, 4> result{};

		result(0, 0) = ( get(1, 1) * c5 - get(1, 2) * c4 + get(1, 3) * c3) * oneOverDeterminant;
		result(0, 1) = (-get(0, 1) * c5 + get(0, 2) * c4 - get(0, 3) * c3) * oneOverDeterminant;
		result(0, 2) = ( get(3, 1) * s5 - get(3, 2) * s4 + get(3, 3) * s3) * oneOverDeterminant;
		result(0, 3) = (-get(2, 1) * s5 + get(2, 2) * s4 - get(2, 3) * s3) * oneOverDeterminant;
		result(1, 0) = (-get(1, 0) * c5 + get(1, 2) * c2 - get(1, 3) * c1) * oneOverDeterminant;
		result(1, 1) = ( get(0, 0) * c5 - get(0, 2) * c2 + get(0, 3) * c1) * oneOverDeterminant;
		result(1, 2) = (-get(3, 0) * s5 + get(3, 2) * s2 - get(3, 3) * s1) * oneOverDeterminant;
		result(1, 3) = ( get(2, 0) * s5 - get(2, 2) * s2 + get(2, 3) * s1) * oneOverDeterminant;
		result(2, 0) = ( get(1, 0) * c4 - get(1, 1) * c2 + get(1, 3) * c0) * oneOverDeterminant;
		result(2, 1) = (-get(0, 0) * c4 + get(0, 1) * c2 - get(0, 3) * c0) * oneOverDeterminant;
		result(2, 2) = ( get(3, 0) * s4 - get(3, 1) * s2 + get(3, 3) * s0) * oneOverDeterminant;
		result(2, 3) = (-get(2, 0) * s4 + get(2, 1) * s2 - get(2, 3) * s0) * oneOverDeterminant;
		result(3, 0) = (-get(1, 0) * c3 + get(1, 1) * c1 - get(1, 2) * c0) * oneOverDeterminant;
		result(3, 1) = ( get(0, 0) * c3 - get(0, 1) * c1 + get(0, 2) * c0) * oneOverDeterminant;
		result(3, 2) = (-get(3, 0) * s3 + get(3, 1) * s1 - get(3, 2) * s0) * oneOverDeterminant;
		result(3, 3) = ( get(2, 0) * s3 - get(2, 1) * s1 + get(2, 2) * s0) * oneOverDeterminant;

		return result;
	}

	template<>
	constexpr Matrix<3, 3> Matrix<3, 3>::affineInverse() const
	{
		return *this;
	}

	template<>
	constexpr Matrix<4, 4> Matrix<4, 4>::affineInverse() const
	{
//		mat<3, 3, T, Q> const Inv(inverse(mat<3, 3, T, Q>(m)));
//
//		return mat<4, 4, T, Q>(
//			vec<4, T, Q>(Inv[0], static_cast<T>(0)),
//			vec<4, T, Q>(Inv[1], static_cast<T>(0)),
//			vec<4, T, Q>(Inv[2], static_cast<T>(0)),
//			vec<4, T, Q>(-Inv * vec<3, T, Q>(m[3]), static_cast<T>(1)));

		return *this;
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr bool
	Matrix<Rows, Columns>::roughlyEquals(Matrix<Rows, Columns> const& lhs, Matrix<Rows, Columns> const& rhs,
										 float epsilon)
	{
		for (matrix_size_type row = 0; row < Rows; row++)
		{
			for (matrix_size_type column = 0; column < Rows; column++)
			{
				float const value1 = lhs.get(row, column);
				float const value2 = rhs.get(row, column);
				if (std::abs(value1 - value2) > epsilon)
				{
					return false;
				}
			}
		}
		return true;
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr Matrix<Rows, Columns> Matrix<Rows, Columns>::operator+(Matrix<Rows, Columns> const& rhs) const
	{
		Matrix<Rows, Columns> result{};
		for (matrix_size_type row = 0; row < Rows; row++)
		{
			for (matrix_size_type column = 0; column < Rows; column++)
			{
				result(row, column) = get(row, column) + rhs.get(row, column);
			}
		}
		return result;
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr Matrix<Rows, Columns> Matrix<Rows, Columns>::operator*(float rhs) const
	{
		Matrix<Rows, Columns> result{};
		for (matrix_size_type row = 0; row < Rows; row++)
		{
			for (matrix_size_type column = 0; column < Rows; column++)
			{
				result(row, column) = get(row, column) * rhs;
			}
		}
		return result;
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr Matrix<Rows, Columns> Matrix<Rows, Columns>::operator-(Matrix<Rows, Columns> const& rhs) const
	{
		Matrix<Rows, Columns> result{};
		for (matrix_size_type row = 0; row < Rows; row++)
		{
			for (matrix_size_type column = 0; column < Rows; column++)
			{
				result(row, column) = get(row, column) - rhs.get(row, column);
			}
		}
		return result;
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr void Matrix<Rows, Columns>::operator+=(Matrix<Rows, Columns> const& rhs)
	{
		for (matrix_size_type row = 0; row < Rows; row++)
		{
			for (matrix_size_type column = 0; column < Rows; column++)
			{
				this->operator()(row, column) += rhs.get(row, column);
			}
		}
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr void Matrix<Rows, Columns>::operator-=(Matrix<Rows, Columns> const& rhs)
	{
		for (matrix_size_type row = 0; row < Rows; row++)
		{
			for (matrix_size_type column = 0; column < Rows; column++)
			{
				this->operator()(row, column) -= rhs.get(row, column);
			}
		}
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr void Matrix<Rows, Columns>::operator*=(float rhs)
	{
		for (matrix_size_type row = 0; row < Rows; row++)
		{
			for (matrix_size_type column = 0; column < Rows; column++)
			{
				this->operator()(row, column) *= rhs;
			}
		}
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr bool Matrix<Rows, Columns>::operator==(Matrix<Rows, Columns> const& rhs) const
	{
		for (matrix_size_type row = 0; row < Rows; row++)
		{
			for (matrix_size_type column = 0; column < Rows; column++)
			{
				if (get(row, column) != rhs.get(row, column))
				{
					return false;
				}
			}
		}
		return true;
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr bool Matrix<Rows, Columns>::operator!=(Matrix<Rows, Columns> const& rhs) const
	{
		return !(this == rhs);
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	template<matrix_size_type rhsRows, matrix_size_type rhsColumns>
	constexpr Matrix<Rows, rhsColumns> Matrix<Rows, Columns>::operator*(Matrix<rhsRows, rhsColumns> const& rhs)
	requires (Columns == rhsRows)
	{
		Matrix<Rows, Columns> result{};
		for (int i = 0; i < Rows; i++)
		{
			for (int j = 0; j < rhsColumns; j++)
			{
				for (int k = 0; k < Columns; k++)
				{
					result._data[i][j] += _data[i][k] * rhs._data[k][j];
				}
			}
		}
		return result;
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	[[nodiscard]] std::string Matrix<Rows, Columns>::toString() const
	{
		std::stringstream result{};
		result << "{";
		for (int i = 0; i < Rows; i++)
		{
			if (i > 0)
			{
				result << " ";
			}
			for (int j = 0; j < Columns; j++)
			{
				result << _data[i][j];
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

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr Matrix<Rows, Columns> Matrix<Rows, Columns>::createIdentity()
	requires (Rows == Columns)
	{
		Matrix<Rows, Columns> result{};
		for (int i = 0; i < Rows; i++)
		{
			result(i, i) = 1.f;
		}
		return result;
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr Matrix<Rows, Columns> Matrix<Rows, Columns>::identity = Matrix<Rows, Columns>::createIdentity();

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr Matrix<Rows, Columns> Matrix<Rows, Columns>::zero = Matrix<Rows, Columns>{};

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr std::ostream& operator<<(std::ostream& ostream, Matrix<Rows, Columns> const& matrix)
	{
		ostream << matrix.toString();
		return ostream;
	}

	template<matrix_size_type Rows, matrix_size_type Columns>
	constexpr Matrix<Rows, Columns> operator*(float lhs, Matrix<Rows, Columns> const& rhs)
	{
		return rhs * lhs;
	}

	// implementation from: https://www.brainvoyager.com/bv/doc/UsersGuide/CoordsAndTransforms/SpatialTransformationMatrices.html
	constexpr static Matrix<4, 4> createTranslationMatrix(Vector<3> translation)
	{
		return Matrix<4, 4>{{{
								 {{1, 0, 0, translation.get(0)}},
								 {{0, 1, 0, translation.get(1)}},
								 {{0, 0, 1, translation.get(2)}},
								 {{0, 0, 0, 1}}
							 }}};
	}

	// implementation from: https://automaticaddison.com/how-to-convert-a-quaternion-to-a-rotation-matrix/
	constexpr static Matrix<4, 4> createRotationMatrix(Quaternion rotation)
	{
		float const x = rotation.x;
		float const y = rotation.y;
		float const z = rotation.z;
		float const w = rotation.w;

		float const m00 = 2 * (x * x + y * y) - 1;
		float const m01 = 2 * (y * z - x * w);
		float const m02 = 2 * (y * w + x * z);

		float const m10 = 2 * (y * z + x * w);
		float const m11 = 2 * (x * x + z * z) - 1;
		float const m12 = 2 * (z * w - x * y);

		float const m20 = 2 * (y * w - x * z);
		float const m21 = 2 * (z * w + x * y);
		float const m22 = 2 * (x * x + w * w) - 1;

		return Matrix<4, 4>{{{
								 {{m00, m01, m02, 0}},
								 {{m10, m11, m12, 0}},
								 {{m20, m21, m22, 0}},
								 {{0, 0, 0, 1}}
							 }}};
	}

	// implementation from https://www.brainvoyager.com/bv/doc/UsersGuide/CoordsAndTransforms/SpatialTransformationMatrices.html
	constexpr static Matrix<4, 4> createScaleMatrix(Vector<3> scale)
	{
		return Matrix<4, 4>{{{
								 {{scale.get(0), 0, 0, 0}},
								 {{0, scale.get(1), 0, 0}},
								 {{0, 0, scale.get(2), 0}},
								 {{0, 0, 0, 1}}
							 }}};
	}

	constexpr static Matrix<4, 4>
	createTranslationRotationScaleMatrix(Vector<3> translation, Quaternion rotation, Vector<3> scale)
	{
		Matrix<4, 4> translationMatrix = createTranslationMatrix(translation);
		Matrix<4, 4> rotationMatrix = createRotationMatrix(rotation);
		Matrix<4, 4> scaleMatrix = createScaleMatrix(scale);

		return translationMatrix * rotationMatrix * scaleMatrix; // todo: is this the right order?
	}

	constexpr static Matrix<4, 4>
	createPerspectiveProjectionMatrix(float fieldOfView, float aspectRatio, float zNear, float zFar)
	{
		float const tanHalfFovy = tan(fieldOfView / 2.0f);

		Matrix<4, 4> result{};
		result(0, 0) = 1.0f / (aspectRatio * tanHalfFovy);
		result(1, 1) = 1.0f / (tanHalfFovy);
		result(2, 2) = zFar / (zFar - zNear);
		result(3, 2) = 1.0f;
		result(2, 3) = -(zFar * zNear) / (zFar - zNear);
		return result;
	}

	constexpr static Matrix<4, 4>
	createOrthographicProjectionMatrix(float left, float right, float top, float bottom, float zNear, float zFar)
	{
		Matrix<4, 4> result{};

		return result;
	}

	constexpr static Matrix<4, 4> createLookAtMatrix(Vector<3> eye, Vector<3> target, Vector<3> worldUp)
	{
		Matrix<4, 4> result{};

		return result;
	}

	constexpr static Matrix<4, 4> invert(Matrix<4, 4> const& matrix)
	{


		return matrix;
	}
}

#endif //BORED_ENGINE_MATRIX_INL
