#pragma once

#include <string>

#include "Angle.h"

namespace ABMath
{
	template<class T>
	class PolarVector2
	{
	public:
		PolarVector2(const T& length, const Angle& angle)
			: _length(length)
			, _angle(angle)
		{}

		T GetLength() const
		{
			return _length;
		}

		void SetLength(const T length)
		{
			_length = length;
		}

		Angle GetAngle() const
		{
			return _angle;
		}

		void SetAngle(const Angle angle)
		{
			_angle = angle;
		}

	private:
		T _length;
		Angle _angle;
	};

	template<class T>
	class PolarCylindricalVector3
	{
	public:
		PolarCylindricalVector3(const T& length, const Angle& angle, const T& height)
			: _vector2(length, angle)
			, _height(height)
		{}

		PolarCylindricalVector3(const PolarVector2<T>& polarVector2, const T& height)
			: _vector2(polarVector2)
			, _height(height)
		{}

		T GetLength() const
		{
			return _vector2.GetLength();
		}

		void SetLength(const T length)
		{
			_vector2.SetLength(length);
		}

		Angle GetAngle() const
		{
			return _vector2.GetAngle();
		}

		void SetAngle(const Angle angle)
		{
			_vector2.SetAngle(angle);
		}

		T GetHeight() const
		{
			return _height;
		}

		void SetHeight(const T& height)
		{
			_height = height;
		}

	private:
		PolarVector2<T> _vector2;
		T _height;
	};

	template<class T>
	class ClassicPolarVector3
	{
	public:
		ClassicPolarVector3(const T& length, const Angle& horAngle, const Angle& vertAngle)
			: _polarVector2(length, horAngle)
			, _verticalAngle(vertAngle)
		{}

		T GetLength() const
		{
			return _polarVector2.GetLength();
		}

		void SetLength(const T length)
		{
			_polarVector2.SetLength(length);
		}

		Angle GetHorizontalAngle() const
		{
			return _polarVector2.GetAngle();
		}

		void SetHorizontalAngle(const Angle& angle)
		{
			_polarVector2.SetAngle(angle);
		}

		Angle GetVerticalAngle() const
		{
			return _verticalAngle;
		}

		void SetVerticalAngle(const Angle& vertAngle)
		{
			_verticalAngle = vertAngle;
		}

	private:
		PolarVector2<T> _polarVector2;
		Angle _verticalAngle;
	};

	template<class T>
	class PolarVector3
	{
	public:
		PolarVector3(const T& length, const Angle& heading, const Angle& pitch)
			: _classicVector3(length, heading, pitch)
		{}

		T GetLength() const
		{
			return _classicVector3.GetLength();
		}

		void SetLength(const T length)
		{
			_classicVector3.SetLength(length);
		}

		Angle GetHeading() const
		{
			return _classicVector3.GetHorizontalAngle();
		}

		void SetHeading(const Angle& heading)
		{
			_classicVector3.SetHorizontalAngle(heading);
		}

		Angle GetPitch() const
		{
			return _classicVector3.GetVerticalAngle();
		}

		void SetPitch(const Angle& pitch)
		{
			_classicVector3.SetVerticalAngle(pitch);
		}

	private:
		ClassicPolarVector3<T> _classicVector3;
	};

	template<class T>
	void ToCanonicalForm(PolarVector2<T>& polarVector)
	{
		const T length = polarVector.GetLength();
		if (length == T(0))
		{
			polarVector.SetAngle(Angle::CreateWithRadians(0.f));
		}
		else
		{
			const Angle pi = Angle::CreatePi();
			Angle angle = polarVector.GetAngle();

			if (length < T(0))
			{
				polarVector.SetLength(-length);
				angle += pi;
			}
			
			if (GetAbsolute(angle) > pi)
			{
				angle += pi;

				const Angle twoPi = Angle::CreateTwoPi();
				angle -= std::floor((angle / twoPi).GetRadians()) * twoPi;

				angle -= pi;
			}

			polarVector.SetAngle(angle);
		}
	}

	template<class T>
	void ToCanonicalForm(ClassicPolarVector3<T>& polarVector)
	{
		const T length = polarVector.GetLength();
		if (length == T(0))
		{
			const Angle zero = Angle::CreateWithRadians(0.f);
			polarVector.SetHorizontalAngle(zero);
			polarVector.SetVerticalAngle(zero);
		}
		else
		{
			const Angle pi = Angle::CreatePi();
			const Angle twoPi = 2.f * pi;
			const Angle piOverTwo = pi / 2.f;

			Angle horAngle = polarVector.GetHorizontalAngle();
			Angle vertAngle = polarVector.GetVerticalAngle();

			if (length < T(0))
			{
				polarVector.SetLength(-length);
				horAngle += pi;
				vertAngle = -vertAngle;
			}

			if (GetAbsolute(vertAngle) > piOverTwo)
			{
				vertAngle += piOverTwo;

				vertAngle -= std::floor((vertAngle / twoPi).GetRadians()) * twoPi;
				if (vertAngle > pi)
				{
					horAngle += pi;
					vertAngle = 3.f * piOverTwo - vertAngle;
				}
				else
				{
					vertAngle -= piOverTwo;
				}

				vertAngle -= piOverTwo;
			}

			if (GetAbsolute(vertAngle) <= 0.0001f)
			{
				horAngle = Angle::CreateWithRadians(0.f);
			}
			else if (GetAbsolute(horAngle) > pi)
			{
				horAngle += pi;

				horAngle -= std::floor((horAngle / twoPi).GetRadians()) * twoPi;

				horAngle -= pi;
			}

			polarVector.SetHorizontalAngle(horAngle);
			polarVector.SetVerticalAngle(vertAngle);
		}
	}

	template<class T>
	void ToCanonicalForm(PolarVector3<T>& polarVector)
	{
		const T length = polarVector.GetLength();
		if (length == T(0))
		{
			const Angle zero = Angle::CreateWithRadians(0.f);
			polarVector.SetHeading(zero);
			polarVector.SetPitch(zero);
		}
		else
		{
			const Angle pi = Angle::CreatePi();
			const Angle twoPi = 2.f * pi;
			const Angle piOverTwo = pi / 2.f;

			Angle heading = polarVector.GetHeading();
			Angle pitch = polarVector.GetPitch();

			if (length < T(0))
			{
				polarVector.SetLength(-length);
				heading += pi;
				pitch = -pitch;
			}

			if (GetAbsolute(pitch) > piOverTwo)
			{
				pitch += piOverTwo;

				pitch -= std::floor((pitch / twoPi).GetRadians()) * twoPi;
				if (pitch > pi)
				{
					heading += pi;
					pitch = 3.f * piOverTwo - pitch;
				}
				else
				{
					pitch -= piOverTwo;
				}

				pitch -= piOverTwo;
			}

			if (GetAbsolute(pitch) >= piOverTwo * 0.9999f)
			{
				heading = Angle::CreateWithRadians(0.f);
			}
			else if (GetAbsolute(heading) > pi)
			{
				heading += pi;

				heading -= std::floor((heading / twoPi).GetRadians()) * twoPi;

				heading -= pi;
			}

			polarVector.SetHeading(heading);
			polarVector.SetPitch(pitch);
		}
	}

	template<class T>
	PolarVector3<T> CreatePolarVector3(const ClassicPolarVector3<T>& classicPolarVector)
	{
		const T length = classicPolarVector.GetLength();
		const Angle heading = -classicPolarVector.GetHorizontalAngle() - Angle::CreateWithDegrees(270.f);
		const Angle pitch = classicPolarVector.GetVerticalAngle() - Angle::CreateWithDegrees(90.f);

		return PolarVector3<T>(length, heading, pitch);
	}

	template<class T>
	PolarVector2<T> CreateCanonicalForm(const PolarVector2<T>& polarVector)
	{
		auto result = polarVector;
		ToCanonicalForm(result);
		return result;
	}

	template<class T>
	PolarVector3<T> CreateCanonicalForm(const PolarVector3<T>& polarVector)
	{
		auto result = polarVector;
		ToCanonicalForm(result);
		return result;
	}

	template<class T>
	std::string ToString(const PolarVector2<T>& vector)
	{
		auto fmt = std::ostringstream();
		
		fmt << std::fixed << std::setprecision(2) << "(" << vector.GetLength() << ", " << GetDegrees<int>(vector.GetAngle()) << ")";

		return fmt.str();
	}

	template<class T>
	std::string ToString(const PolarCylindricalVector3<T>& vector)
	{
		auto fmt = std::ostringstream();

		fmt << std::fixed << std::setprecision(2) << "("
			<< vector.GetLength()
			<< ", " << GetDegrees<int>(vector.GetAngle())
			<< ", " << vector.GetHeight()
			<< ")";

		return fmt.str();
	}

	template<class T>
	std::string ToString(const ClassicPolarVector3<T>& vector)
	{
		auto fmt = std::ostringstream();

		fmt << std::fixed << std::setprecision(2) << "("
			<< vector.GetLength()
			<< ", " << GetDegrees<int>(vector.GetHorizontalAngle())
			<< ", " << GetDegrees<int>(vector.GetVerticalAngle())
			<< ")";

		return fmt.str();
	}

	template<class T>
	std::string ToString(const PolarVector3<T>& vector)
	{
		auto fmt = std::ostringstream();

		fmt << std::fixed << std::setprecision(2) << "("
			<< vector.GetLength()
			<< ", " << GetDegrees<int>(vector.GetHeading())
			<< ", " << GetDegrees<int>(vector.GetPitch())
			<< ")";

		return fmt.str();
	}
}