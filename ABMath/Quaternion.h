#pragma once

#include <string>

namespace ABMath
{
	class Quaternion
	{
	public:
		Quaternion(const float w, const float x, const float y, const float z);

		float GetW() const;
		float GetX() const;
		float GetY() const;
		float GetZ() const;

		void Conjugate();

	private:
		float _w;
		float _x;
		float _y;
		float _z;
	};

	float Magnitude(const Quaternion& quaternion);

	Quaternion CreateConjugate(const Quaternion& quaternion);

	void Fill(const Quaternion& quaternion, float& w, float& x, float& y, float& z);

	std::string ToString(const Quaternion& quaternion);
}



