#pragma once

#include <string>
#include <vector>

#include "Angle.h"

namespace ABMath
{
	class EulerAngles
	{
	public:
		template<class T>
		static EulerAngles CreateWithDegrees(const T& heading, const T& pitch, const T& bank)
		{
			return EulerAngles(Angle::CreateWithDegrees(heading), Angle::CreateWithDegrees(pitch), Angle::CreateWithDegrees(bank));
		}

	public:
		EulerAngles();
		EulerAngles(const Angle& heading, const Angle& pitch, const Angle& bank);

		Angle GetHeading() const;
		void SetHeading(const Angle& angle);

		Angle GetPitch() const;
		void SetPitch(const Angle& angle);

		Angle GetBank() const;
		void SetBank(const Angle& angle);

	private:
		Angle _heading;
		Angle _pitch;
		Angle _bank;
	};

	bool IsInGimbalLockState(const EulerAngles& eulerAngles);

	std::string ToString(const EulerAngles& eulerAngles);
	bool IsCanonical(const EulerAngles& eulerAngles, std::vector<std::string>* outDetails);
}