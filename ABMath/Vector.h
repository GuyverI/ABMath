#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace ABMath
{
	template<class T, size_t SIZE>
	class Vector
	{
	public:
		using DataType = std::vector<T>;

	public:
		static Vector Zero()
		{
			return Vector();
		}

		explicit Vector(const DataType& data)
			: _data()
		{
			const size_t inDataSize = data.size();
			for (size_t i = 0; i < SIZE; ++i)
			{
				if (i < inDataSize)
				{
					_data.push_back(data[i]);
				}
				else
				{
					_data.push_back(T(0));
				}
			}
		}

		const T& At(const size_t index) const
		{
			return _data[index];
		}

		T& At(const size_t index)
		{
			const auto& constThis = static_cast<const Vector&>(*this);
			return const_cast<T&>(constThis.At(index));
		}

	private:
		Vector()
			: _data()
		{
			for (size_t i = 0; i < SIZE; ++i)
			{
				_data.push_back(T(0));
			}
		}

	private:
		DataType _data;
	};

	template<size_t SIZE>
	using FVector = Vector<float, SIZE>;
	using FVector2 = FVector<2>;
	using FVector3 = FVector<3>;
	using FVector4 = FVector<4>;

	template<size_t SIZE>
	using IVector = Vector<int, SIZE>;
	using IVector2 = IVector<2>;
	using IVector3 = IVector<3>;
	using IVector4 = IVector<4>;

	template<class T>
	Vector<T, 4> ToVector4(const Vector<T, 3>& vector, const T& w)
	{
		return Vector<T, 4>({ vector.At(0), vector.At(1), vector.At(2), w });
	}

	template<class T, size_t SIZE>
	std::string ToString(const Vector<T, SIZE>& vector)
	{
		auto fmt = std::ostringstream();
		fmt << "[";
		for (size_t i = 0; i < SIZE; ++i)
		{
			fmt << std::fixed << std::setprecision(2) << vector.At(i);
			if ((i + 1) < SIZE)
			{
				fmt << ", ";
			}
		}
		fmt << "]";

		return fmt.str();
	}

	template<class T, size_t SIZE>
	Vector<T, SIZE> Multiply(const Vector<T, SIZE>& vec, const T& val)
	{
		auto result = vec;
		for (size_t i = 0; i < SIZE; ++i)
		{
			result.At(i) *= val;
		}

		return result;
	}

	template<class T, size_t SIZE>
	Vector<T, SIZE> Divide(const Vector<T, SIZE>& vec, const T& val)
	{
		return Multiply(vec, T(1) / val);
	}

	template<class T>
	Vector<T, 2> Vector3ToVector2(const Vector<T, 3>& vector3)
	{
		return Vector<T, 2>({ vector3.At(0), vector3.At(1) });
	}

	template<class T, size_t SIZE>
	float Length(const Vector<T, SIZE>& vector)
	{
		float length = 0;
		for (size_t i = 0; i < SIZE; ++i)
		{
			const T& val = vector.At(i);
			length += val * val;
		}

		return std::sqrt(length);
	}

	template<class T, size_t SIZE>
	Vector<T, SIZE> CreateNormalized(const Vector<T, SIZE>& vector)
	{
		const float length = Length(vector);

		return Divide(vector, T(length));
	}
}

