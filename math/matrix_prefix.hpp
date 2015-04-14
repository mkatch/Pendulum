#ifndef JG_MATH_MATRIX_PREFIX_HPP
#define JG_MATH_MATRIX_PREFIX_HPP

namespace jg {
namespace math {

template <typename T>
inline T
det(
	T a00, T a10,
	T a01, T a11
)
{
	return a00 * a11 - a10 * a01;
}

template <typename T>
inline T
det(
	T a00, T a10, T a20,
	T a01, T a11, T a21,
	T a02, T a12, T a22
)
{
	return a00 * det(a11, a21,
					 a12, a22)
		 - a01 * det(a10, a20,
		 			 a12, a22)
		 + a02 * det(a10, a20,
		 			 a11, a21);
}

template <typename T>
inline T
det(
	T a00, T a10, T a20, T a30,
	T a01, T a11, T a21, T a31,
	T a02, T a12, T a22, T a32,
	T a03, T a13, T a23, T a33
)
{
	return a00 * det(a11, a21, a31,
					 a12, a22, a32,
					 a13, a23, a33)
		 - a01 * det(a10, a20, a30,
		 			 a12, a22, a32,
					 a13, a23, a33)
		 + a02 * det(a10, a20, a30,
		 			 a11, a21, a31,
					 a13, a23, a33)
		 - a03 * det(a10, a20, a30,
		 			 a11, a21, a31,
					 a12, a22, a32);
}

}; /* Namespace math. */
}; /* Namespace jg. */

#endif /* JG_MATH_MATRIX_PREFIX_HPP */
