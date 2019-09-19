#pragma once

#include <string>

#include "Angle.h"
#include "Vector.h"

namespace ABMath
{
	template <class T>
	class AxisAngle
	{
	public:
		template <class T>
		AxisAngle(const Vector<T, 3>& axis, const Angle& angle)
			: _axis(axis)
			, _angle(angle)
		{}

		Angle GetAngle() const
		{
			return _angle;
		}

		Vector<T, 3> GetAxis() const
		{
			return _axis;
		}

	private:
		Vector<T, 3> _axis;
		Angle _angle;
	};
}



