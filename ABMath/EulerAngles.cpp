#include "EulerAngles.h"

#include "Float.h"

namespace ABMath
{
	EulerAngles::EulerAngles()
		: EulerAngles(Angle::Zero(), Angle::Zero(), Angle::Zero())
	{}

	EulerAngles::EulerAngles(const Angle& heading, const Angle& pitch, const Angle& bank)
		: _heading(heading)
		, _pitch(pitch)
		, _bank(bank)
	{
		if (IsInGimbalLockState(*this))
		{
			_bank = Angle::Zero();
		}
	}

	Angle EulerAngles::GetHeading() const
	{
		return _heading;
	}

	void EulerAngles::SetHeading(const Angle& angle)
	{
		_heading = angle;
	}

	Angle EulerAngles::GetPitch() const
	{
		return _pitch;
	}

	void EulerAngles::SetPitch(const Angle& angle)
	{
		_pitch = angle;

		if (IsInGimbalLockState(*this))
		{
			_bank = Angle::Zero();
		}
	}

	Angle EulerAngles::GetBank() const
	{
		return _bank;
	}

	void EulerAngles::SetBank(const Angle& angle)
	{
		if (!IsInGimbalLockState(*this))
			_bank = angle;
	}



	bool IsInGimbalLockState(const EulerAngles& eulerAngles)
	{
		return AreFloatsEqual(std::fabs(eulerAngles.GetPitch().GetRadians()), Angle::PI_OVER_TWO);
	}

	std::string ToString(const EulerAngles& eulerAngles)
	{
		const int heading = GetDegrees<int>(eulerAngles.GetHeading());
		const int pitch = GetDegrees<int>(eulerAngles.GetPitch());
		const int bank = GetDegrees<int>(eulerAngles.GetBank());
		return "(" + std::to_string(heading) + ", " + std::to_string(pitch) + ", " + std::to_string(bank) + ")";
	}

	bool IsCanonical(const EulerAngles& eulerAngles, std::vector<std::string>* outDetails)
	{
		const auto isInPiRange = [](const Angle& angle)
		{
			const Angle minusPi = -Angle::CreatePi();
			const Angle pi = Angle::CreatePi();
			return minusPi < angle && angle <= pi;
		};

		const auto isInPiOverTwoRange = [](const Angle& angle)
		{
			const Angle minusPiOverTwo = -Angle::CreatePiOverTwo();
			const Angle piOverTwo = Angle::CreatePiOverTwo();
			return minusPiOverTwo <= angle && angle <= piOverTwo;
		};

		const bool isCanonicalHeading = isInPiRange(eulerAngles.GetHeading());
		const bool isCanonicalPitch = isInPiOverTwoRange(eulerAngles.GetPitch());
		const bool isCanonicalBank = isInPiRange(eulerAngles.GetBank());

		if (outDetails)
		{
			if (!isCanonicalHeading)
				outDetails->push_back("Heading angle is out of canonical range");
			if (!isCanonicalPitch)
				outDetails->push_back("Pitch angle is out of canonical range");
			if (!isCanonicalBank)
				outDetails->push_back("Bank angle is out of canonical range");
		}

		return isCanonicalHeading && isCanonicalPitch && isCanonicalBank;
	}
}