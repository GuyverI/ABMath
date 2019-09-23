#include "Quaternion.h"

#include <cmath>
#include <sstream>

#include "Float.h"
#include "Vector.h"

namespace ABMath
{
	Quaternion::Quaternion(const float w, const float x, const float y, const float z)
		: _w(w)
		, _x(x)
		, _y(y)
		, _z(z)
	{}

	float Quaternion::GetW() const
	{
		return _w;
	}

	float Quaternion::GetX() const
	{
		return _x;
	}

	float Quaternion::GetY() const
	{
		return _y;
	}

	float Quaternion::GetZ() const
	{
		return _z;
	}

	void Quaternion::Conjugate()
	{
		_x = -_x;
		_y = -_y;
		_z = -_z;
	}

	void Quaternion::Inverse()
	{
		const float magnitude = Magnitude(*this);
		Conjugate();
		_w /= magnitude;
		_x /= magnitude;
		_y /= magnitude;
		_z /= magnitude;
	}

	void Quaternion::Exponentiate(const float exponent)
	{
		if (!IsIdentity(*this))
		{
			const float alpha = std::acos(_w);
			const float newAlpha = alpha * exponent;
			
			_w = std::cos(newAlpha);

			const float multiplier = std::sin(newAlpha) / std::sin(alpha);
			_x *= multiplier;
			_y *= multiplier;
			_z *= multiplier;
		}
	}



	bool IsIdentity(const Quaternion& quaternion)
	{
		return AreFloatsEqual(1.f, quaternion.GetW());
	}

	float Magnitude(const Quaternion& quaternion)
	{
		const auto w = quaternion.GetW();
		const auto x = quaternion.GetX();
		const auto y = quaternion.GetY();
		const auto z = quaternion.GetZ();
		return std::sqrt(w*w + x*x + y*y + z*z);
	}

	Quaternion CreateConjugate(const Quaternion& quaternion)
	{
		auto result = quaternion;
		result.Conjugate();
		return result;
	}

	Quaternion CreateExponentiated(const Quaternion& quaternion, const float exponent)
	{
		Quaternion result = quaternion;
		result.Exponentiate(exponent);
		return result;
	}

	float DotProduct(const Quaternion& left, const Quaternion& right)
	{
		return left.GetW() * right.GetW()
			+ left.GetX() * right.GetX()
			+ left.GetY() * right.GetY()
			+ left.GetZ() * right.GetZ();
	}

	Quaternion Multiply(const Quaternion& left, const Quaternion& right)
	{
		const auto leftW = left.GetW();
		const auto rightW = right.GetW();
		const auto leftVector = FVector3({ left.GetX(), left.GetY(), left.GetZ() });
		const auto rightVector = FVector3({ right.GetX(), right.GetY(), right.GetZ() });
		
		const auto w = leftW * rightW - DotProduct(leftVector, rightVector);

		auto vector = Multiply(rightVector, leftW);
		Add(vector, Multiply(leftVector, rightW));
		Add(vector, CrossProduct(leftVector, rightVector));

		return Quaternion(w, vector.At(0), vector.At(1), vector.At(2));
	}

	Quaternion Subtract(const Quaternion& left, Quaternion right)
	{
		right.Inverse();
		return Multiply(left, right);
	}

	void Fill(const Quaternion& quaternion, float& w, float& x, float& y, float& z)
	{
		w = quaternion.GetW();
		x = quaternion.GetX();
		y = quaternion.GetY();
		z = quaternion.GetZ();
	}
	
	std::string ToString(const Quaternion& quaternion)
	{
		auto stream = std::ostringstream();
		stream << "(" << quaternion.GetW() << ", [" << quaternion.GetX() << ", " << quaternion.GetY() << ", " << quaternion.GetZ() << "])";
		return stream.str();
	}

}
