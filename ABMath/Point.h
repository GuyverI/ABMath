#pragma  once

#include "Vector.h"

namespace ABMath
{
	template<class T, size_t SIZE>
	class Point
	{
	public:
		static Point Origin()
		{
			return Point();
		}

	public:
		Point(const T& x, const T& y, const T& z)
			: _vector(Vector<T, SIZE>::DataType({ x, y, z }))
		{
			static_assert(SIZE > 0);
		}

		Point(const T& x, const T& y)
			: Point(x, y, T())
		{}

		const T& X() const
		{
			return _vector.At(0);
		}
		void X(const T& x)
		{
			_vector.At(0) = x;
		}

		const T& Y() const
		{
			static_assert(SIZE > 1);
			return _vector.At(1);
		}
		void Y(const T& y)
		{
			static_assert(SIZE > 1);
			_vector.At(1) = y;
		}

		const T& Z() const
		{
			static_assert(SIZE > 2);
			return _vector.At(2);
		}
		void Z(const T& z)
		{
			static_assert(SIZE > 2);
			_vector.At(2) = z;
		}

		void Add(const Vector<T, SIZE>& vector)
		{
			for (size_t i = 0; i < SIZE; ++i)
			{
				_vector.At(i) += vector.At(i);
			}
		}

		Vector<T, SIZE> Subtract(const Point<T, SIZE>& other) const
		{
			auto vecData = Vector<T, SIZE>::DataType(SIZE);
			for (size_t i = 0; i < SIZE; ++i)
			{
				vecData[i] = _vector.At(i) - other._vector.At(i);
			}

			return Vector<T, SIZE>(vecData);
		}

	private:
		Vector<T, SIZE> _vector;
	};

	template<size_t SIZE>
	using FPoint = Point<float, SIZE>;
	using FPoint2 = FPoint<2>;
	using FPoint3 = FPoint<3>;
	using FPoint4 = FPoint<4>;

	template<size_t SIZE>
	using IPoint = Point<int, SIZE>;
	using IPoint2 = IPoint<2>;
	using IPoint3 = IPoint<3>;
	using IPoint4 = IPoint<4>;


	template <class T, size_t SIZE>
	void operator+=(Point<T, SIZE>& point, const Vector<T, SIZE>& vector)
	{
		point.Add(point, vector);
	}

	template <class T, size_t SIZE>
	Point<T, SIZE> Add(const Point<T, SIZE>& point, const Vector<T, SIZE>& vector)
	{
		auto newPoint = point;
		newPoint.Add(vector);
		return newPoint;
	}
	template <class T, size_t SIZE>
	Point<T, SIZE> operator+(const Point<T, SIZE>& point, const Vector<T, SIZE>& vector)
	{
		return Add(point, vector);
	}

	template <class T, size_t SIZE>
	Vector<T, SIZE> operator-(const Point<T, SIZE>& left, const Point<T, SIZE>& right)
	{
		return left.Subtract(right);
	}
}

