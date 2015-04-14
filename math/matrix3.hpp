#ifndef JG_MATH_MATRIX3_HPP
#define JG_MATH_MATRIX3_HPP

#include <iostream>
#include <cmath>
#include "matrix_prefix.hpp"
#include "tabproxy.hpp"

namespace jg {
namespace math {

/*
 * a00 a10 a20
 * a01 a11 a21
 * a02 a12 a22
 *
 * t[0] t[3] t[6]
 * t[1] t[4] t[7]
 * t[2] t[5] t[8]
 *
 */

template <typename T>
class matrix3
{
  public:
	T t[9];

	matrix3();
  explicit
	matrix3(T s);
	matrix3(
		T a00, T a10, T a20,
		T a01, T a11, T a21,
		T a02, T a12, T a22
	);
  template <typename S>
  	matrix3(matrix3<S> const& m);

	matrix3& operator += (matrix3 const& m);
	matrix3& operator -= (matrix3 const& m);
	matrix3& operator *= (matrix3 const& m);
	matrix3& operator *= (T s);
	matrix3& operator /= (T s);
	matrix3  operator -  () const;

	T det() const;
	void transpose();
	matrix3 transposed() const;
	void invert();
	matrix3 inverse() const;

	tabProxy<T>       operator [] (int col);
	tabProxy<T const> operator [] (int col) const;
};

typedef matrix3<int>    int3x3;
typedef matrix3<float>  float3x3;
typedef matrix3<double> double3x3;

template <typename T>
inline matrix3<T>
operator + (matrix3<T> const& a, matrix3<T> const& b)
{
	return matrix3<T>(
		a.t[0] + b.t[0], a.t[3] + b.t[3], a.t[6] + b.t[6],
		a.t[1] + b.t[1], a.t[4] + b.t[4], a.t[7] + b.t[7],
		a.t[2] + b.t[2], a.t[5] + b.t[5], a.t[8] + b.t[8]
	);
}

template <typename T, typename S>
inline matrix3<T>
operator + (matrix3<T> const& a, matrix3<S> const& b)
{
	return a + static_cast<matrix3<T> >(b);
}

template <typename T>
inline matrix3<T>
operator - (matrix3<T> const& a, matrix3<T> const& b)
{
	return matrix3<T>(
		a.t[0] - b.t[0], a.t[3] - b.t[3], a.t[6] - b.t[6],
		a.t[1] - b.t[1], a.t[4] - b.t[4], a.t[7] - b.t[7],
		a.t[2] - b.t[2], a.t[5] - b.t[5], a.t[8] - b.t[8]
	);
}

template <typename T, typename S>
inline matrix3<T>
operator - (matrix3<T> const& a, matrix3<S> const& b)
{
	return a - static_cast<matrix3<T> >(b);
}

template <typename T>
inline matrix3<T>
operator * (matrix3<T> const& a, matrix3<T> const& b)
{
	return matrix3<T>(
		a.t[0] * b.t[0] + a.t[3] * b.t[1] + a.t[6] * b.t[2],
		a.t[0] * b.t[3] + a.t[3] * b.t[4] + a.t[6] * b.t[5],
		a.t[0] * b.t[6] + a.t[3] * b.t[7] + a.t[6] * b.t[8],

		a.t[1] * b.t[0] + a.t[4] * b.t[1] + a.t[7] * b.t[2],
		a.t[1] * b.t[3] + a.t[4] * b.t[4] + a.t[7] * b.t[5],
		a.t[1] * b.t[6] + a.t[4] * b.t[7] + a.t[7] * b.t[8],

		a.t[2] * b.t[0] + a.t[5] * b.t[1] + a.t[8] * b.t[2],
		a.t[2] * b.t[3] + a.t[5] * b.t[4] + a.t[8] * b.t[5],
		a.t[2] * b.t[6] + a.t[5] * b.t[7] + a.t[8] * b.t[8]
	);
}

template <typename T, typename S>
inline matrix3<T>
operator * (matrix3<T> const& a, matrix3<S> const& b)
{
	return a * static_cast<matrix3<T> >(b);
}

template <typename T, typename S>
inline matrix3<T>
operator * (matrix3<T> const& m, S s)
{
	T const cs = static_cast<T>(s);
	return matrix3<T>(
		m.t[0] * cs, m.t[3] * cs, m.t[6] * cs,
		m.t[1] * cs, m.t[4] * cs, m.t[7] * cs,
		m.t[2] * cs, m.t[5] * cs, m.t[8] * cs
	);
}

template <typename T, typename S>
inline matrix3<T>
operator * (S s, matrix3<T> const& m)
{
	return m * s;
}

template <typename T, typename S>
inline matrix3<T>
operator / (matrix3<T> const& m, S s)
{
	T const cs = static_cast<T>(s);
	return matrix3<T>(
		m.t[0] / cs, m.t[3] / cs, m.t[6] / cs,
		m.t[1] / cs, m.t[4] / cs, m.t[7] / cs,
		m.t[2] / cs, m.t[5] / cs, m.t[8] / cs
	);
}

template <typename T>
inline
matrix3<T>::matrix3()
{/* Do nothing. */}

template <typename T>
inline
matrix3<T>::matrix3(T s)
{
	t[0] = t[1] = t[2] = t[3] = t[4] = t[5] = t[6] = t[7] = t[8] = s;
}

template <typename T>
inline
matrix3<T>::matrix3(
	T a00, T a10, T a20,
	T a01, T a11, T a21,
	T a02, T a12, T a22
)
{
	t[0] = a00; t[3] = a10; t[6] = a20;
	t[1] = a01; t[4] = a11; t[7] = a21;
	t[2] = a02; t[5] = a12; t[8] = a22;
}

template <typename T>
template <typename S>
inline
matrix3<T>::matrix3(matrix3<S> const& m)
{
	t[0] = static_cast<T>(m.t[0]);
	t[1] = static_cast<T>(m.t[1]);
	t[2] = static_cast<T>(m.t[2]);
	t[3] = static_cast<T>(m.t[3]);
	t[4] = static_cast<T>(m.t[4]);
	t[5] = static_cast<T>(m.t[5]);
	t[6] = static_cast<T>(m.t[6]);
	t[7] = static_cast<T>(m.t[7]);
	t[8] = static_cast<T>(m.t[8]);
}

template <typename T>
inline matrix3<T>&
matrix3<T>::operator += (matrix3<T> const& m)
{
	t[0] += m.t[0];
	t[1] += m.t[1];
	t[2] += m.t[2];
	t[3] += m.t[3];
	t[4] += m.t[4];
	t[5] += m.t[5];
	t[6] += m.t[6];
	t[7] += m.t[7];
	t[8] += m.t[8];
	return *this;
}

template <typename T>
inline matrix3<T>&
matrix3<T>::operator -= (matrix3<T> const& m)
{
	t[0] -= m.t[0];
	t[1] -= m.t[1];
	t[2] -= m.t[2];
	t[3] -= m.t[3];
	t[4] -= m.t[4];
	t[5] -= m.t[5];
	t[6] -= m.t[6];
	t[7] -= m.t[7];
	t[8] -= m.t[8];
	return *this;
}

template <typename T>
inline matrix3<T>&
matrix3<T>::operator *= (matrix3<T> const& m)
{
	T a00 = t[0] * m.t[0] + t[3] * m.t[1] + t[6] * m.t[2];
	T a01 = t[0] * m.t[3] + t[3] * m.t[4] + t[6] * m.t[5];
	T a02 = t[0] * m.t[6] + t[3] * m.t[7] + t[6] * m.t[8];

	T a10 = t[1] * m.t[0] + t[4] * m.t[1] + t[7] * m.t[2];
	T a11 = t[1] * m.t[3] + t[4] * m.t[4] + t[7] * m.t[5];
	T a12 = t[1] * m.t[6] + t[4] * m.t[7] + t[7] * m.t[8];

	T a20 = t[2] * m.t[0] + t[5] * m.t[1] + t[8] * m.t[2];
	T a21 = t[2] * m.t[3] + t[5] * m.t[4] + t[8] * m.t[5];
	T a22 = t[2] * m.t[6] + t[5] * m.t[7] + t[8] * m.t[8];
	
	t[0] = a00; t[3] = a10; t[6] = a20;
	t[1] = a01; t[4] = a11; t[7] = a21;
	t[2] = a02; t[5] = a12; t[8] = a22;

	return *this;
}

template <typename T>
inline matrix3<T>&
matrix3<T>::operator *= (T s)
{
	t[0] *= s;
	t[1] *= s;
	t[2] *= s;
	t[3] *= s;
	t[4] *= s;
	t[5] *= s;
	t[6] *= s;
	t[7] *= s;
	t[8] *= s;
	
	return *this;
}

template <typename T>
inline matrix3<T>&
matrix3<T>::operator /= (T s)
{
	t[0] /= s;
	t[1] /= s;
	t[2] /= s;
	t[3] /= s;
	t[4] /= s;
	t[5] /= s;
	t[6] /= s;
	t[7] /= s;
	t[8] /= s;

	return *this;
}

template <typename T>
inline T
matrix3<T>::det() const
{
	return jg::math::det(
		t[0], t[3], t[6],
		t[1], t[4], t[7],
		t[2], t[5], t[8]
	);
}

template <typename T>
inline void
matrix3<T>::invert()
{
	*this = *this.inverse();
}

template <typename T>
inline matrix3<T>
matrix3<T>::inverse() const
{
	T d = static_cast<T>(1.0) / det();
	return matrix3<T>(
		d * jg::math::det(t[4], t[7],
						  t[5], t[8]),
	   -d * jg::math::det(t[3], t[6],
	   					  t[5], t[8]),
		d * jg::math::det(t[3], t[6],
			    		  t[4], t[7]),

	   -d * jg::math::det(t[1], t[7],
	   					  t[2], t[8]),
		d * jg::math::det(t[0], t[6],
						  t[2], t[8]),
	   -d * jg::math::det(t[0], t[6],
	   					  t[1], t[7]),

		d * jg::math::det(t[1], t[4],
						  t[2], t[5]),
	   -d * jg::math::det(t[0], t[3],
	   					  t[2], t[5]),
		d * jg::math::det(t[0], t[3],
						  t[1], t[4])
	);
}

template <typename T>
inline void
matrix3<T>::transpose()
{
	std::swap(t[1], t[3]);
	std::swap(t[2], t[6]);
	std::swap(t[5], t[7]);
}

template <typename T>
inline matrix3<T>
matrix3<T>::transposed() const
{
	return matrix3<T>(
		t[0], t[1], t[2],
		t[3], t[4], t[5],
		t[6], t[7], t[8]
	);
}

template <typename T>
inline matrix3<T>
matrix3<T>::operator - () const
{
	return matrix3<T>(
		-t[0], -t[3], -t[6],
		-t[1], -t[4], -t[7],
		-t[2], -t[5], -t[8]
	);
}

template <typename T>
inline std::ostream&
operator << (std::ostream& ostr, matrix3<T> const& m)
{
	return ostr << '|' << m.t[0] << ' ' << m.t[3] << ' ' << m.t[6] << "|\n"
				<< '|' << m.t[1] << ' ' << m.t[4] << ' ' << m.t[7] << "|\n"
				<< '|' << m.t[2] << ' ' << m.t[5] << ' ' << m.t[8] << '|';
}

template <typename T>
inline std::istream&
operator >> (std::istream& istr, matrix3<T>& m)
{
	return istr >> m.t[0] >> m.t[3] >> m.t[6]
				>> m.t[1] >> m.t[4] >> m.t[7]
				>> m.t[2] >> m.t[5] >> m.t[8];
}

template <typename T>
inline tabProxy<T>
matrix3<T>::operator [] (int col)
{
	return tabProxy<T>(t + 3 * col);
}

template <typename T>
inline tabProxy<T const>
matrix3<T>::operator [] (int col) const
{
	return tabProxy<T const>(t + 3 * col);
}

} /* Namespace math. */
} /* Namescpace jg. */

#endif /* JG_MATH_MATRIX3_HPP */
