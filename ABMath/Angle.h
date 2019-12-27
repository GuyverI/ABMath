#pragma once

namespace ABMath
{
	class Angle
	{
	public:
		constexpr static float PI = 3.14159265358979323846f;
		constexpr static float PI_OVER_TWO = PI / 2.f;
		constexpr static float TWO_PI = PI * 2.f;

		template<class T>
		static Angle CreateWithDegrees(const T& deg)
		{
			return Angle(DegreesToRadians(deg));
		}

		static Angle CreateWithRadians(const float rad);

		static Angle CreatePi();

		static Angle CreateTwoPi();

		static Angle CreatePiOverTwo();

		static Angle Zero();

		float GetRadians() const;

		Angle operator-() const;

		Angle& operator+=(const Angle& other);

		Angle& operator-=(const Angle& other);

		Angle& operator/=(const Angle& other);

		template<class T>
		Angle& operator*=(const T& factor)
		{
			_radians *= factor;
			return *this;
		}

		template<class T>
		Angle& operator/=(const T& factor)
		{
			_radians /= factor;
			return *this;
		}

	private:
		template<class T>
		static constexpr float DegreesToRadians(const T& deg)
		{
			return static_cast<float>(deg) * PI / 180.f;
		}

		Angle() = default;
		explicit Angle(const float radians);

	private:
		float _radians;
	};

	template<class T>
	T GetDegrees(const Angle& angle)
	{
		return static_cast<T>(angle.GetRadians() * 180.f / Angle::CreatePi().GetRadians());
	}

	bool operator<(const Angle& left, const Angle& right);

	bool operator>(const Angle& left, const Angle& right);

	bool operator==(const Angle& left, const Angle& right);

	bool operator!=(const Angle& left, const Angle& right);

	bool operator<=(const Angle& left, const Angle& right);
	bool operator>=(const Angle& left, const Angle& right);

	Angle operator+(const Angle& left, const Angle& right);

	Angle operator-(const Angle& left, const Angle& right);

	Angle operator/(const Angle& left, const Angle& right);

	template<class T>
	Angle operator*(const Angle& angle, const T& factor)
	{
		Angle result = angle;
		result *= factor;
		return result;
	}

	template<class T>
	Angle operator*(const T& factor, const Angle& angle)
	{
		return angle * factor;
	}

	template<class T>
	Angle operator/(const Angle& angle, const T& factor)
	{
		Angle result = angle;
		result /= factor;
		return result;
	}

	Angle GetAbsolute(const Angle& angle);
}