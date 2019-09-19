#include "Quaternion.h"

#include <cmath>
#include <sstream>

#include "Float.h"

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
