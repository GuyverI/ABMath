#include <limits>

namespace ABMath
{
	template<class T>
	bool AreFloatsEqual(const T& left, const T& right)
	{
		return std::abs(left - right) < std::numeric_limits<T>::epsilon();
	}

	template<class T>
	bool IsFloatEqualOrLarger(const T& left, const T& right)
	{
		return AreFloatsEqual(left, right) || left > right;
	}
}