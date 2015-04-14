#ifndef JG_MATH_HPP
#define JG_MATH_HPP

#include "vector.hpp"
//#include "matrix.hpp"
//#include "quaternion.hpp"

namespace jg {
namespace math {

double const PI		= 3.14159265358979323846;
double const PI_2	= 1.57079632679489661923;
double const PI_4	= 0.78539816339744830962;

template <typename T>
inline T
sq(T a)
{
	return a * a;
}

} // namespace math
} // namespace jg

#endif /* JG_MATH_HPP */
