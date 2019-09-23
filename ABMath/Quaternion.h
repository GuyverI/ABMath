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
		void Inverse();
		void Exponentiate(const float exponent);

	private:
		float _w;
		float _x;
		float _y;
		float _z;
	};

	bool IsIdentity(const Quaternion& quaternion);

	float Magnitude(const Quaternion& quaternion);

	Quaternion CreateConjugate(const Quaternion& quaternion);
	Quaternion CreateExponentiated(const Quaternion& quaternion, const float exponent);

	float DotProduct(const Quaternion& left, const Quaternion& right);
	Quaternion Multiply(const Quaternion& left, const Quaternion& right);
	Quaternion Subtract(const Quaternion& left, Quaternion right);

	void Fill(const Quaternion& quaternion, float& w, float& x, float& y, float& z);

	std::string ToString(const Quaternion& quaternion);
}



