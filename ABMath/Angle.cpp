#include "Angle.h"

#include "Float.h"

namespace ABMath
{
	Angle Angle::CreateWithRadians(const float rad)
	{
		return Angle(rad);
	}

	Angle Angle::CreatePi()
	{
		return Angle(PI);
	}

	Angle Angle::CreateTwoPi()
	{
		Angle res = CreatePi();
		res *= 2;
		return res;
	}

	Angle Angle::CreatePiOverTwo()
	{
		Angle res = CreatePi();
		res /= 2;
		return res;
	}

	Angle Angle::Zero()
	{
		return Angle(0.f);
	}

	float Angle::GetRadians() const
	{
		return _radians;
	}

	Angle Angle::operator-() const
	{
		return Angle(-_radians);
	}

	Angle& Angle::operator+=(const Angle& other)
	{
		_radians += other.GetRadians();
		return *this;
	}

	Angle& Angle::operator-=(const Angle& other)
	{
		*this += -other;
		return *this;
	}

	Angle& Angle::operator/=(const Angle& other)
	{
		_radians /= other.GetRadians();
		return *this;
	}

	Angle::Angle(const float radians)
		: _radians(radians)
	{}



	bool operator<(const Angle& left, const Angle& right)
	{
		if (left != right)
		{
			return left.GetRadians() < right.GetRadians();
		}
		
		return false;
	}

	bool operator>(const Angle& left, const Angle& right)
	{
		return right < left;
	}

	bool operator==(const Angle& left, const Angle& right)
	{
		return AreFloatsEqual(left.GetRadians(), right.GetRadians());
	}

	bool operator!=(const Angle& left, const Angle& right)
	{
		return !operator==(left, right);
	}

	bool operator<=(const Angle& left, const Angle& right)
	{
		return left < right || left == right;
	}

	bool operator>=(const Angle& left, const Angle& right)
	{
		return left > right || left == right;
	}

	Angle operator+(const Angle& left, const Angle& right)
	{
		Angle result = left;
		result += right;
		return result;
	}

	Angle operator-(const Angle& left, const Angle& right)
	{
		return left + (-right);
	}

	Angle operator/(const Angle& left, const Angle& right)
	{
		Angle result = left;
		result /= right;
		return result;
	}

	Angle GetAbsolute(const Angle& angle)
	{
		if (angle > Angle::Zero())
		{
			return angle;
		}

		return -angle;
	}
}