#ifndef JG_MATH_MATRIX4_HPP
#define JG_MATH_MATRIX4_HPP

#include <iostream>
#include <cmath>
#include "matrix_prefix.hpp"
#include "tabproxy.hpp"

namespace jg {
namespace math {

/*
 * a00 a10 a20 a30
 * a01 a11 a21 a31
 * a02 a12 a22 a32
 * a03 a13 a23 a33
 *
 * t[ 0] t[ 4] t[ 8] t[12]
 * t[ 1] t[ 5] t[ 9] t[13]
 * t[ 2] t[ 6] t[10] t[14]
 * t[ 3] t[ 7] t[11] t[15]
 *
 */

template <typename T>
class matrix4
{
  public:
  	static matrix4<T> identity();

	T t[16];

	matrix4();
  explicit
	matrix4(T s);
	matrix4(
		T a00, T a10, T a20, T a30,
		T a01, T a11, T a21, T a31,
		T a02, T a12, T a22, T a32,
		T a03, T a13, T a23, T a33
	);
  template <typename S>
  	matrix4(matrix4<S> const& m);

	matrix4& operator += (matrix4 const& m);
	matrix4& operator -= (matrix4 const& m);
	matrix4& operator *= (matrix4 const& m);
	matrix4& operator *= (T s);
	matrix4& operator /= (T s);
	matrix4  operator -  () const;

	T det() const;
	void transpose();
	matrix4 transposed() const;
	void invert();
	matrix4 inverse() const;

	tabProxy<T>       operator [] (int col);
	tabProxy<T const> operator [] (int col) const;
};

typedef matrix4<int>    int4x4;
typedef matrix4<float>  float4x4;
typedef matrix4<double> double4x4;

template <typename T>
inline matrix4<T>
matrix4<T>::identity()
{
	return matrix4<T>(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
}

template <typename T>
inline matrix4<T>
operator + (matrix4<T> const& a, matrix4<T> const& b)
{
	return matrix4<T>(
		a.t[0] + b.t[0], a.t[4] + b.t[4], a.t[ 8] + b.t[ 8], a.t[12] + b.t[12],
		a.t[1] + b.t[1], a.t[5] + b.t[5], a.t[ 9] + b.t[ 9], a.t[13] + b.t[13],
		a.t[2] + b.t[2], a.t[6] + b.t[6], a.t[10] + b.t[10], a.t[14] + b.t[14],
		a.t[3] + b.t[3], a.t[7] + b.t[7], a.t[11] + b.t[11], a.t[15] + b.t[15]
	);
}

template <typename T, typename S>
inline matrix4<T>
operator + (matrix4<T> const& a, matrix4<S> const& b)
{
	return a + static_cast<matrix4<T> >(b);
}

template <typename T>
inline matrix4<T>
operator - (matrix4<T> const& a, matrix4<T> const& b)
{
	return matrix4<T>(
		a.t[0] - b.t[0], a.t[4] - b.t[4], a.t[ 8] - b.t[ 8], a.t[12] - b.t[12],
		a.t[1] - b.t[1], a.t[5] - b.t[5], a.t[ 9] - b.t[ 9], a.t[13] - b.t[13],
		a.t[2] - b.t[2], a.t[6] - b.t[6], a.t[10] - b.t[10], a.t[14] - b.t[14],
		a.t[3] - b.t[3], a.t[7] - b.t[7], a.t[11] - b.t[11], a.t[15] - b.t[15]
	);
}

template <typename T, typename S>
inline matrix4<T>
operator - (matrix4<T> const& a, matrix4<S> const& b)
{
	return a - static_cast<matrix4<T> >(b);
}

template <typename T>
inline matrix4<T>
operator * (matrix4<T> const& a, matrix4<T> const& b)
{
	return matrix4<T>(
	a.t[0] * b.t[ 0] + a.t[4] * b.t[ 1] + a.t[ 8] * b.t[ 2] + a.t[12] * b.t[ 3],
	a.t[0] * b.t[ 4] + a.t[4] * b.t[ 5] + a.t[ 8] * b.t[ 6] + a.t[12] * b.t[ 7],
	a.t[0] * b.t[ 8] + a.t[4] * b.t[ 9] + a.t[ 8] * b.t[10] + a.t[12] * b.t[11],
	a.t[0] * b.t[12] + a.t[4] * b.t[13] + a.t[ 8] * b.t[14] + a.t[12] * b.t[15],
	
	a.t[1] * b.t[ 0] + a.t[5] * b.t[ 1] + a.t[ 9] * b.t[ 2] + a.t[13] * b.t[ 3],
	a.t[1] * b.t[ 4] + a.t[5] * b.t[ 5] + a.t[ 9] * b.t[ 6] + a.t[13] * b.t[ 7],
	a.t[1] * b.t[ 8] + a.t[5] * b.t[ 9] + a.t[ 9] * b.t[10] + a.t[13] * b.t[11],
	a.t[1] * b.t[12] + a.t[5] * b.t[13] + a.t[ 9] * b.t[14] + a.t[13] * b.t[15],
	
	a.t[2] * b.t[ 0] + a.t[6] * b.t[ 1] + a.t[10] * b.t[ 2] + a.t[14] * b.t[ 3],
	a.t[2] * b.t[ 4] + a.t[6] * b.t[ 5] + a.t[10] * b.t[ 6] + a.t[14] * b.t[ 7],
	a.t[2] * b.t[ 8] + a.t[6] * b.t[ 9] + a.t[10] * b.t[10] + a.t[14] * b.t[11],
	a.t[2] * b.t[12] + a.t[6] * b.t[13] + a.t[10] * b.t[14] + a.t[14] * b.t[15],
	
	a.t[3] * b.t[ 0] + a.t[7] * b.t[ 1] + a.t[11] * b.t[ 2] + a.t[15] * b.t[ 3],
	a.t[3] * b.t[ 4] + a.t[7] * b.t[ 5] + a.t[11] * b.t[ 6] + a.t[15] * b.t[ 7],
	a.t[3] * b.t[ 8] + a.t[7] * b.t[ 9] + a.t[11] * b.t[10] + a.t[15] * b.t[11],
	a.t[3] * b.t[12] + a.t[7] * b.t[13] + a.t[11] * b.t[14] + a.t[15] * b.t[15]
	);
}

template <typename T, typename S>
inline matrix4<T>
operator * (matrix4<T> const& a, matrix4<S> const& b)
{
	return a * static_cast<matrix4<T> >(b);
}

template <typename T, typename S>
inline matrix4<T>
operator * (matrix4<T> const& m, S s)
{
	T const cs = static_cast<T>(s);
	return matrix4<T>(
		m.t[0] * cs, m.t[4] * cs, m.t[ 8] * cs, m.t[12] * cs,
		m.t[1] * cs, m.t[5] * cs, m.t[ 9] * cs, m.t[13] * cs,
		m.t[2] * cs, m.t[6] * cs, m.t[10] * cs, m.t[14] * cs,
		m.t[3] * cs, m.t[7] * cs, m.t[11] * cs, m.t[15] * cs
	);
}

template <typename T, typename S>
inline matrix4<T>
operator * (S s, matrix4<T> const& m)
{
	return m * s;
}

template <typename T, typename S>
inline matrix4<T>
operator / (matrix4<T> const& m, S s)
{
	return m * (static_cast<T>(1) / static_cast<T>(s));
}

template <typename T>
inline
matrix4<T>::matrix4()
{/* Do nothing. */}

template <typename T>
inline
matrix4<T>::matrix4(T s)
{
	t[0] = t[1] = t[2] = t[3] = t[4] = t[5] = t[6] = t[7] = t[8] = t[9] = t[10]
		= t[11] = t[12] = t[13] = t[14] = s;
}

template <typename T>
inline
matrix4<T>::matrix4(
	T a00, T a10, T a20, T a30,
	T a01, T a11, T a21, T a31,
	T a02, T a12, T a22, T a32,
	T a03, T a13, T a23, T a33
)
{
	t[0] = a00; t[4] = a10; t[ 8] = a20; t[12] = a30;
	t[1] = a01; t[5] = a11; t[ 9] = a21; t[13] = a31;
	t[2] = a02; t[6] = a12; t[10] = a22; t[14] = a32;
	t[3] = a03; t[7] = a13; t[11] = a23; t[15] = a33;
}

template <typename T>
template <typename S>
inline
matrix4<T>::matrix4(matrix4<S> const& m)
{
	t[ 0] = static_cast<T>(m.t[ 0]);
	t[ 1] = static_cast<T>(m.t[ 1]);
	t[ 2] = static_cast<T>(m.t[ 2]);
	t[ 3] = static_cast<T>(m.t[ 3]);
	t[ 4] = static_cast<T>(m.t[ 4]);
	t[ 5] = static_cast<T>(m.t[ 5]);
	t[ 6] = static_cast<T>(m.t[ 6]);
	t[ 7] = static_cast<T>(m.t[ 7]);
	t[ 8] = static_cast<T>(m.t[ 8]);
	t[ 9] = static_cast<T>(m.t[ 9]);
	t[10] = static_cast<T>(m.t[10]);
	t[11] = static_cast<T>(m.t[11]);
	t[12] = static_cast<T>(m.t[12]);
	t[13] = static_cast<T>(m.t[13]);
	t[14] = static_cast<T>(m.t[14]);
	t[15] = static_cast<T>(m.t[15]);
}

template <typename T>
inline matrix4<T>&
matrix4<T>::operator += (matrix4<T> const& m)
{
	t[ 0] += m.t[ 0];
	t[ 1] += m.t[ 1];
	t[ 2] += m.t[ 2];
	t[ 3] += m.t[ 3];
	t[ 4] += m.t[ 4];
	t[ 5] += m.t[ 5];
	t[ 6] += m.t[ 6];
	t[ 7] += m.t[ 7];
	t[ 8] += m.t[ 8];
	t[ 9] += m.t[ 9];
	t[10] += m.t[10];
	t[11] += m.t[11];
	t[12] += m.t[12];
	t[13] += m.t[13];
	t[14] += m.t[14];
	t[15] += m.t[15];

	return *this;
}

template <typename T>
inline matrix4<T>&
matrix4<T>::operator -= (matrix4<T> const& m)
{
	t[ 0] -= m.t[ 0];
	t[ 1] -= m.t[ 1];
	t[ 2] -= m.t[ 2];
	t[ 3] -= m.t[ 3];
	t[ 4] -= m.t[ 4];
	t[ 5] -= m.t[ 5];
	t[ 6] -= m.t[ 6];
	t[ 7] -= m.t[ 7];
	t[ 8] -= m.t[ 8];
	t[ 9] -= m.t[ 9];
	t[10] -= m.t[10];
	t[11] -= m.t[11];
	t[12] -= m.t[12];
	t[13] -= m.t[13];
	t[14] -= m.t[14];
	t[15] -= m.t[15];

	return *this;
}

template <typename T>
inline matrix4<T>&
matrix4<T>::operator *= (matrix4<T> const& m)
{
	return *this = *this * m;
}

template <typename T>
inline matrix4<T>&
matrix4<T>::operator *= (T s)
{
	t[ 0] *= s;
	t[ 1] *= s;
	t[ 2] *= s;
	t[ 3] *= s;
	t[ 4] *= s;
	t[ 5] *= s;
	t[ 6] *= s;
	t[ 7] *= s;
	t[ 8] *= s;
	t[ 9] *= s;
	t[10] *= s;
	t[11] *= s;
	t[12] *= s;
	t[13] *= s;
	t[14] *= s;
	t[15] *= s;

	return *this;
}

template <typename T>
inline matrix4<T>&
matrix4<T>::operator /= (T s)
{
	return operator *= (static_cast<T>(1) / s);
}

template <typename T>
inline T
matrix4<T>::det() const
{
	return jg::math::det(
		t[0], t[4], t[ 8], t[12],
		t[1], t[5], t[ 9], t[13],
		t[2], t[6], t[10], t[14],
		t[3], t[7], t[11], t[15]
	);
}

template <typename T>
inline void
matrix4<T>::invert()
{
	*this = *this.inverse();
}

template <typename T>
inline matrix4<T>
matrix4<T>::inverse() const
{
	using jg::math::det;
	T d = static_cast<T>(1) / this->det();
	return matrix4<T>(
		d * det(t[ 5], t[ 9], t[13],
				t[ 6], t[10], t[14],
				t[ 7], t[11], t[15]),
	   -d * det(t[ 4], t[ 8], t[12],
				t[ 6], t[10], t[14],
				t[ 7], t[11], t[15]),
		d * det(t[ 4], t[ 8], t[12],
				t[ 5], t[ 9], t[13],
				t[ 7], t[11], t[15]),
	   -d * det(t[ 4], t[ 8], t[12],
				t[ 5], t[ 9], t[13],
				t[ 6], t[10], t[14]),

	   -d * det(t[ 1], t[ 9], t[13],
				t[ 2], t[10], t[14],
				t[ 3], t[11], t[15]),
		d * det(t[ 0], t[ 8], t[12],
				t[ 2], t[10], t[14],
				t[ 3], t[11], t[15]),
	   -d * det(t[ 0], t[ 8], t[12],
				t[ 1], t[ 9], t[13],
				t[ 3], t[11], t[15]),
		d * det(t[ 0], t[ 8], t[12],
				t[ 1], t[ 9], t[13],
				t[ 2], t[10], t[14]),
		
		d * det(t[ 1], t[ 5], t[13],
				t[ 2], t[ 6], t[14],
				t[ 3], t[ 7], t[15]),
	   -d * det(t[ 0], t[ 4], t[12],
				t[ 2], t[ 6], t[14],
				t[ 3], t[ 7], t[15]),
		d * det(t[ 0], t[ 4], t[12],
				t[ 1], t[ 5], t[13],
				t[ 3], t[ 7], t[15]),
	   -d * det(t[ 0], t[ 4], t[12],
				t[ 1], t[ 5], t[13],
				t[ 2], t[ 6], t[14]),
		
	   -d * det(t[ 1], t[ 5], t[ 9],
				t[ 2], t[ 6], t[10],
				t[ 3], t[ 7], t[11]),
	 	d * det(t[ 0], t[ 4], t[ 8],
				t[ 2], t[ 6], t[10],
				t[ 3], t[ 7], t[11]),
	   -d * det(t[ 0], t[ 4], t[ 8],
				t[ 1], t[ 5], t[ 9],
				t[ 3], t[ 7], t[11]),
		d * det(t[ 0], t[ 4], t[ 8],
				t[ 1], t[ 5], t[ 9],
				t[ 2], t[ 6], t[10])
	);
}

template <typename T>
inline void
matrix4<T>::transpose()
{
	*this = transposed();
}

template <typename T>
inline matrix4<T>
matrix4<T>::transposed() const
{
	return matrix4<T>(
		t[ 0], t[ 1], t[ 2], t[ 3],
		t[ 4], t[ 5], t[ 6], t[ 7],
		t[ 8], t[ 9], t[10], t[11],
		t[12], t[13], t[14], t[15]
	);
}

template <typename T>
inline matrix4<T>
matrix4<T>::operator - () const
{
	return matrix4<T>(
		-t[0], -t[4], -t[ 8], -t[12],
		-t[1], -t[5], -t[ 9], -t[13],
		-t[2], -t[6], -t[10], -t[14],
		-t[3], -t[7], -t[11], -t[15]
	);
}

template <typename T>
inline std::ostream&
operator << (std::ostream& ostr, matrix4<T> const& m)
{
	return ostr << '|' << m.t[ 0] << ' ' << m.t[4] << ' ' << m.t[ 8]
				<< ' ' << m.t[12] << "|\n"
				<< '|' << m.t[ 1] << ' ' << m.t[5] << ' ' << m.t[ 9]
				<< ' ' << m.t[13] << "|\n"
				<< '|' << m.t[ 2] << ' ' << m.t[6] << ' ' << m.t[10]
				<< ' ' << m.t[14] << "|\n"
				<< '|' << m.t[ 3] << ' ' << m.t[7] << ' ' << m.t[11]
				<< ' ' << m.t[15] << '|';
}

template <typename T>
inline std::istream&
operator >> (std::istream& istr, matrix4<T>& m)
{
	return istr >> m.t[0] >> m.t[4] >> m.t[ 8] >> m.t[12]
				>> m.t[1] >> m.t[5] >> m.t[ 9] >> m.t[13]
				>> m.t[2] >> m.t[6] >> m.t[10] >> m.t[14]
				>> m.t[3] >> m.t[7] >> m.t[11] >> m.t[15];
}

template <typename T>
inline tabProxy<T>
matrix4<T>::operator [] (int col)
{
	return tabProxy<T>(t + 4 * col);
}

template <typename T>
inline tabProxy<T const>
matrix4<T>::operator [] (int col) const
{
	return tabProxy<T const>(t + 4 * col);
}

} /* Namespace math. */
} /* Namescpace jg. */

#endif /* JG_MATH_MATRIX4_HPP */
