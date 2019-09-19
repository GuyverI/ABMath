#include "Utilities.h"

namespace ABMath
{
	EulerAngles QuaternionToEulerAngles(const Quaternion& quaternion)
	{
		auto result = EulerAngles();

		const auto w = quaternion.GetW();
		const auto x = quaternion.GetX();
		const auto y = quaternion.GetY();
		const auto z = quaternion.GetZ();

		const float pitchSin = -2.f * (y*z - w*x);
		if (IsFloatEqualOrLarger(pitchSin, 1.f))
		{
			result.SetPitch(Angle::CreatePiOverTwo());

			const float headingRad = std::atan2(-x*z + w*y, 0.5f - y*y - z*z);
			result.SetHeading(Angle::CreateWithRadians(headingRad));
		}
		else
		{
			result.SetPitch(Angle::CreateWithRadians(std::asin(pitchSin)));

			const float headingRad = std::atan2(x*z + w*y, 0.5f - x*x - y*y);
			result.SetHeading(Angle::CreateWithRadians(headingRad));

			const float bankRad = std::atan2(x*y + w*z, 0.5f - x*x - z*z);
			result.SetBank(Angle::CreateWithRadians(bankRad));
		}

		return result;
	}

	FMatrix3 QuaternionToMatrix(const Quaternion& quaternion)
	{
		auto result = FMatrix3();

		const auto w = quaternion.GetW();
		const auto x = quaternion.GetX();
		const auto y = quaternion.GetY();
		const auto z = quaternion.GetZ();

		result.At(0, 0) = 1 - 2.f*(y*y) - 2.f*(z*z);
		result.At(0, 1) = 2.f*(x*y) + 2.f*(w*z);
		result.At(0, 2) = 2.f*(x*z) - 2.f*(w*y);

		result.At(1, 0) = 2.f*(x*y) - 2.f*(w*z);
		result.At(1, 1) = 1 - 2.f*(x*x) - 2.f*(z*z);
		result.At(1, 2) = 2.f*(y*z) + 2.f*(w*x);

		result.At(2, 0) = 2.f*(x*z) + 2.f*(w*y);
		result.At(2, 1) = 2.f*(y*z) - 2.f*(w*x);
		result.At(2, 2) = 1 - 2.f*(x*x) - 2.f*(y*y);

		return result;
	}
}