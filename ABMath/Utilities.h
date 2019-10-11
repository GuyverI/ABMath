#pragma once

#include <cmath>

#include "AxisAngle.h"
#include "EulerAngles.h"
#include "Matrix.h"
#include "PolarVector.h"
#include "Vector.h"
#include "Quaternion.h"

namespace ABMath
{
	template<class T, size_t SIZE>
	Angle CalculateAngleBetweenClockwise(const Vector<T, SIZE>& vector1, const Vector<T, SIZE>& vector2)
	{
		const auto crossProduct = CrossProduct(vector1, vector2);
		const float sinOfAngle = Length(crossProduct);

		const float dotProduct = DotProduct(vector1, vector2);
		
		const float radians = std::atan2(sinOfAngle, dotProduct);

		return Angle::CreateWithRadians(radians);
	}

	template<typename T>
	Angle CalculateAngleBetweenClockwise(const Vector<T, 2>& vector1, const Vector<T, 2>& vector2)
	{
		const float crossProduct = vector1.At(0) * vector2.At(1) - vector1.At(1) * vector2.At(0);
		const float dotProduct = DotProduct(vector1, vector2);

		const float radians = std::atan2(crossProduct, dotProduct);

		return Angle::CreateWithRadians(radians);
	}

	template<class T, size_t SIZE>
	Vector<T, SIZE> Multiply(const Vector<T, SIZE>& vec, const Matrix<T, SIZE>& mat)
	{
		auto result = Vector<T, SIZE>::Zero();
		for (size_t resultCol = 0; resultCol < SIZE; ++resultCol)
		{
			for (size_t sourceCol = 0; sourceCol < SIZE; ++sourceCol)
			{
				result.At(resultCol) += vec.At(sourceCol) * mat.At(sourceCol, resultCol);
			}
		}

		return result;
	}

	template<class T>
	Vector<T, 3> ProjectOntoX(const Vector<T, 3>& vec, const T& x)
	{
		auto vec4 = ToVector4(vec, T(1));
		auto projMatrix = CreatePerspectiveProjectionX(x);
		const T denominator = Multiply(vec4, projMatrix).At(3);
		return Divide(vec, denominator);
	}

	template<class T>
	Vector<T, 2> PolarVectorToVector(const PolarVector2<T>& polarVector)
	{
		const T radius = polarVector.GetLength();
		const float angle = polarVector.GetAngle().GetRadians();
		const T x = static_cast<T>(radius * std::cos(angle));
		const T y = static_cast<T>(radius * std::sin(angle));

		return Vector<T, 2>({ x, y });
	}

	template<class T>
	Vector<T, 3> PolarVectorToVector(const PolarCylindricalVector3<T>& polarVector)
	{
		const T radius = polarVector.GetLength();
		const float angle = polarVector.GetAngle().GetRadians();
		const T x = static_cast<T>(radius * std::cos(angle));
		const T y = static_cast<T>(radius * std::sin(angle));

		return Vector<T, 3>({ x, y, polarVector.GetHeight() });
	}

	template<class T>
	Vector<T, 3> PolarVectorToVector(const ClassicPolarVector3<T> polarVector)
	{
		const T length = polarVector.GetLength();
		const float horAngle = polarVector.GetHorizontalAngle().GetRadians();
		const float vertAngle = polarVector.GetVerticalAngle().GetRadians();
		const T x = static_cast<T>(length * std::sin(vertAngle) * std::cos(horAngle));
		const T y = static_cast<T>(length * std::sin(vertAngle) * std::sin(horAngle));
		const T z = static_cast<T>(length * std::cos(vertAngle));

		return Vector<T, 3>({ x, y, z });
	}

	template<class T>
	Vector<T, 3> PolarVectorToVector(const PolarVector3<T>& polarVector)
	{
		const T length = polarVector.GetLength();
		const float heading = polarVector.GetHeading().GetRadians();
		const float pitch = polarVector.GetPitch().GetRadians();
		const T x = static_cast<T>(length * std::cos(pitch) * std::sin(heading));
		const T y = static_cast<T>(-length * std::sin(pitch));
		const T z = static_cast<T>(length * std::cos(pitch) * std::cos(heading));

		return Vector<T, 3>({ x, y, z });
	}

	template<class T>
	PolarVector2<T> VectorToPolarVector(const Vector<T, 2>& vector)
	{
		const T x = vector.At(0);
		const T y = vector.At(1);
		
		const T radius = T(std::hypot(x, y));
		const float angle = std::atan2f(static_cast<float>(y), static_cast<float>(x));

		return PolarVector2<T>(radius, ABMath::Angle::CreateWithRadians(angle));
	}

	template<class T>
	PolarCylindricalVector3<T> VectorToPolarCylindricalVector(const Vector<T, 3>& vector)
	{
		auto vector2 = Vector3ToVector2(vector);
		auto polarVector2 = VectorToPolarVector(vector2);
		return PolarCylindricalVector3<T>(polarVector2, vector.At(2));
	}

	template<class T>
	PolarVector3<T> VectorToPolarVector(const Vector<T, 3>& vector)
	{
		const T x = vector.At(0);
		const T y = vector.At(1);
		const T z = vector.At(2);

		const auto length = T(std::hypot(x, y, z));
		float heading = 0.f;
		float pitch = 0.f;
		if (length > T(0))
		{
			const float piOverTwo = Angle::PI / 2.f;
			pitch = static_cast<float>(std::asin(-y / length));
			if (std::fabs(pitch) < piOverTwo * 0.9999f)
			{
				heading = static_cast<float>(std::atan2(x, z));
			}
		}

		return PolarVector3<T>(length, Angle::CreateWithRadians(heading), Angle::CreateWithRadians(pitch));
	}

	template<class T>
	EulerAngles MatrixToEulerAngles(const Matrix<T, 3>& matrix)
	{
		auto result = EulerAngles();

		const float sinOfPitch = -matrix.At(2, 1);

		if (sinOfPitch <= -1.0f)
			result.SetPitch(-Angle::CreatePiOverTwo());
		else if (sinOfPitch >= 1.0f)
			result.SetPitch(Angle::CreatePiOverTwo());
		else
			result.SetPitch(Angle::CreateWithRadians(std::asin(sinOfPitch)));

		if (fabs(result.GetPitch().GetRadians()) == Angle::PI_OVER_TWO)
		{
			result.SetHeading(Angle::CreateWithRadians(std::atan2(-matrix.At(0, 2), matrix.At(0, 0))));
		}
		else
		{
			result.SetHeading(Angle::CreateWithRadians(std::atan2(matrix.At(2, 0), matrix.At(2, 2))));
			result.SetBank(Angle::CreateWithRadians(std::atan2(matrix.At(0, 1), matrix.At(1, 1))));
		}

		return result;
	}

	EulerAngles QuaternionToEulerAngles(const Quaternion& quaternion);

	FMatrix3 QuaternionToMatrix(const Quaternion& quaternion);

	template<class T>
	Quaternion AxisAngleToQuaternion(const AxisAngle<T>& axisAngle)
	{
		const Angle halfAngle = axisAngle.GetAngle() / 2.f;

		const float w = std::cos(halfAngle.GetRadians());

		const auto normalizedAxis = CreateNormalized(axisAngle.GetAxis());
		const float sinHalfAngle = std::sin(halfAngle.GetRadians());
		const float x = normalizedAxis.At(0) * sinHalfAngle;
		const float y = normalizedAxis.At(1) * sinHalfAngle;
		const float z = normalizedAxis.At(2) * sinHalfAngle;

		return Quaternion(w, x, y, z);
	}
}
