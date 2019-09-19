#pragma once

#include <cmath>
#include <functional>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "Angle.h"
#include "Float.h"

namespace ABMath
{
	template<class T, size_t SIZE>
	class Matrix final
	{
	public:
		using RowType = std::vector<T>;
		using DataType = std::vector<RowType>;

	public:
		static Matrix Identity()
		{
			auto result = Matrix();

			ForEach(result, [&result](const size_t row, const size_t col)
			{
				if (col == row)
				{
					result.At(row, col) = 1;
				}
			});

			return result;
		}

		Matrix()
		{
			for (size_t row = 0; row < SIZE; ++row)
			{
				_data.push_back(RowType());
				for (size_t col = 0; col < SIZE; ++col)
				{
					_data.back().push_back(T());
				}
			}
		}

		explicit Matrix(DataType data)
			: _data(data)
		{}

		const T& At(const size_t row, const size_t col) const
		{
			return _data[row][col];
		}

		T& At(const size_t row, const size_t col)
		{
			const auto& constThis = static_cast<const Matrix&>(*this);
			return const_cast<T&>(constThis.At(row, col));
		}
		

	private:
		DataType _data;
	};

	template<size_t SIZE>
	using IMatrix = Matrix<int, SIZE>;
	using IMatrix2 = IMatrix<2>;
	using IMatrix3 = IMatrix<3>;
	using IMatrix4 = IMatrix<4>;

	template<size_t SIZE>
	using FMatrix = Matrix<float, SIZE>;
	using FMatrix2 = FMatrix<2>;
	using FMatrix3 = FMatrix<3>;
	using FMatrix4 = FMatrix<4>;

	template<class T, size_t SIZE>
	void ForEach(Matrix<T, SIZE>& matrix, const std::function<void(const size_t, const size_t)>& func)
	{
		for (size_t row = 0; row < SIZE; ++row)
		{
			for (size_t col = 0; col < SIZE; ++col)
			{
				func(row, col);
			}
		}
	}

	template<class T, size_t SIZE>
	bool AreEqual(const Matrix<T, SIZE>& left, const Matrix<T, SIZE>& right, const T epsilon = std::numeric_limits<T>::epsilon())
	{
		for (size_t row = 0; row < SIZE; ++row)
		{
			for (size_t col = 0; col < SIZE; ++col)
			{
				if (std::numeric_limits<T>::is_integer)
				{
					if (left.At(row, col) != right.At(row, col))
					{
						return false;
					}
				}
				else
				{
					if (!AreFloatsEqual(left.At(row, col), right.At(row, col)))
					{
						return false;
					}
				}
			}
		}

		return true;
	}

	template<class T, size_t SIZE>
	Matrix<T, SIZE> Multiply(const Matrix<T, SIZE>& left, const Matrix<T, SIZE>& right)
	{
		auto matrix = Matrix<T, SIZE>();

		ForEach(matrix, [&matrix, &left, &right](const size_t row, const size_t col)
		{
			for (size_t c = 0; c < SIZE; ++c)
			{
				matrix.At(row, col) += left.At(row, c) * right.At(c, col);
			}
		});

		return matrix;
	}

	template<class T, size_t SIZE>
	Matrix<T, SIZE> Divide(const Matrix<T, SIZE>& matrix, const T& divider)
	{
		auto result = Matrix<T, SIZE>();

		ForEach(result, [&result, &matrix, &divider](const size_t row, const size_t col)
		{
			result.At(row, col) = matrix.At(row, col) / divider;
		});

		return result;
	}

	template<class T>
	T Determinant(const Matrix<T, 2>& matrix)
	{
		return matrix.At(0, 0) * matrix.At(1, 1) - matrix.At(0, 1) * matrix.At(1, 0);
	}

	template<class T, size_t SIZE>
	T Minor(const Matrix<T, SIZE>& matrix, const size_t row, const size_t col)
	{
		using SubMatrix = Matrix<T, SIZE - 1>;
		auto subMatrixData = SubMatrix::DataType();

		for (size_t r = 0; r < SIZE; ++r)
		{
			if (r == row) { continue; }

			auto rowData = SubMatrix::RowType();
			for (size_t c = 0; c < SIZE; ++c)
			{
				if (c == col) { continue; }

				rowData.push_back(matrix.At(r, c));
			}
			subMatrixData.push_back(rowData);
		}

		return Determinant(SubMatrix(subMatrixData));
	}

	template<class T, size_t SIZE>
	T Cofactor(const Matrix<T, SIZE>& matrix, const size_t row, const size_t col)
	{
		return static_cast<T>(std::pow(-1, row + col)) * Minor(matrix, row, col);
	}

	template<class T, size_t SIZE>
	T Determinant(const Matrix<T, SIZE>& matrix)
	{
		const size_t row = 0;

		auto sum = T();
		for (size_t col = 0; col < SIZE; ++col)
		{
			sum += matrix.At(row, col) * Cofactor(matrix, row, col);
		}

		return sum;
	}

	template<class T, size_t SIZE>
	void Transpose(Matrix<T, SIZE>& matrix)
	{
		//TODO: AB = foreach
		for (size_t row = 0; row < SIZE; ++row)
		{
			for (size_t col = 0; col < SIZE; ++col)
			{
				if (col > row)
				{
					std::swap(matrix.At(row, col), matrix.At(col, row));
				}
			}
		}
	}

	template<class T, size_t SIZE>
	Matrix<T, SIZE> Adjoint(const Matrix<T, SIZE>& matrix)
	{
		auto result = Matrix<T, SIZE>();

		//TODO: AB = foreach
		for (size_t row = 0; row < SIZE; ++row)
		{
			for (size_t col = 0; col < SIZE; ++col)
			{
				result.At(row, col) = Cofactor(matrix, row, col);
			}
		}

		Transpose(result);

		return result;
	}

	template<class T, size_t SIZE>
	Matrix<T, SIZE> Inverse(const Matrix<T, SIZE>& matrix)
	{
		auto adj = Adjoint(matrix);
		const T det = Determinant(matrix);

		return Divide(adj, det);
	}

	template<class T, size_t SIZE>
	bool IsOrthogonal(const Matrix<T, SIZE>& matrix, const T epsilon = std::numeric_limits<T>::epsilon())
	{
		auto mT =  matrix;
		Transpose(mT);
		auto identity = Matrix<T, SIZE>::Identity();

		return AreEqual(Multiply<T, SIZE>(mT, matrix), identity, epsilon);
	}

	template<class T>
	void SetTranslation(Matrix<T, 4>& matrix, const T& x, const T& y, const T& z)
	{
		matrix.At(3, 0) = x;
		matrix.At(3, 1) = y;
		matrix.At(3, 2) = z;
	}

	template<class T>
	Matrix<T, 4> CreateTranslation(const T& x, const T& y, const T& z)
	{
		auto matrix = Matrix<T, 4>::Identity();
		SetTranslation(matrix, x, y, z);
		return matrix;
	}

	template<class T, size_t SIZE>
	void SetRotationX(Matrix<T, SIZE>& matrix, const Angle& angle)
	{
		if (SIZE > 2)
		{
			matrix.At(0, 0) = T(1);
			matrix.At(0, 1) = T(0);
			matrix.At(0, 2) = T(0);
			matrix.At(1, 0) = T(0);
			matrix.At(1, 1) = std::cos(angle.GetRadians());
			matrix.At(1, 2) = std::sin(angle.GetRadians());
			matrix.At(2, 0) = T(0);
			matrix.At(2, 1) = -std::sin(angle.GetRadians());
			matrix.At(2, 2) = std::cos(angle.GetRadians());
		}
	}

	template<class T, size_t SIZE>
	void SetRotationY(Matrix<T, SIZE>& matrix, const Angle& angle)
	{
		if (SIZE > 2)
		{
			matrix.At(0, 0) = std::cos(angle.GetRadians());
			matrix.At(0, 1) = T(0);
			matrix.At(0, 2) = -std::sin(angle.GetRadians());
			matrix.At(1, 0) = T(0);
			matrix.At(1, 1) = T(1);
			matrix.At(1, 2) = T(0);
			matrix.At(2, 0) = std::sin(angle.GetRadians());
			matrix.At(2, 1) = T(0);
			matrix.At(2, 2) = std::cos(angle.GetRadians());
		}
	}

	template<class T, size_t SIZE>
	void SetRotationZ(Matrix<T, SIZE>& matrix, const Angle& angle)
	{
		if (SIZE > 2)
		{
			matrix.At(0, 0) = std::cos(angle.GetRadians());
			matrix.At(0, 1) = std::sin(angle.GetRadians());
			matrix.At(0, 2) = T(0);
			matrix.At(1, 0) = -std::sin(angle.GetRadians());
			matrix.At(1, 1) = std::cos(angle.GetRadians());
			matrix.At(1, 2) = T(0);
			matrix.At(2, 0) = T(0);
			matrix.At(2, 1) = T(0);
			matrix.At(2, 2) = T(1);
		}
	}

	template<class T, size_t SIZE>
	Matrix<T, SIZE> CreateRotationX(const Angle& angle)
	{
		auto matrix = Matrix<T, SIZE>::Identity();
		SetRotationX(matrix, angle);
		return matrix;
	}

	template<class T>
	void SetPerspectiveProjectionX(Matrix<T, 4>& matrix, const T& x)
	{
		if (x != T(0))
		{
			matrix.At(0, 3) = T(1) / x;
			matrix.At(3, 3) = T(0);
		}
	}
	template<class T>
	Matrix<T, 4> CreatePerspectiveProjectionX(const T& x)
	{
		auto matrix = Matrix<T, 4>::Identity();
		SetPerspectiveProjectionX(matrix, x);

		return matrix;
	}

	template<class T, size_t SIZE>
	std::string ToString(const Matrix<T, SIZE>& matrix)
	{
		auto fmt = std::ostringstream();

		for (size_t row = 0; row < SIZE; ++row)
		{
			for (size_t col = 0; col < SIZE; ++col)
			{
				const T& val = matrix.At(row, col);
				const auto space = (val < 0) ? std::string("  ") : std::string("   ");
				fmt << std::fixed << std::setprecision(2) << val << space;
			}
			fmt << "\n";
		}

		return fmt.str();
	}
}

