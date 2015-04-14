#ifndef JG_MATH_MATRIX2_HPP
#define JG_MATH_MATRIX2_HPP

#include <iostream>
#include <cmath>
#include "matrix_prefix.hpp"
#include "tabproxy.hpp"

namespace jg {
namespace math {

/*
 * a00 a10
 * a01 a11
 *
 * t[0] t[2]
 * t[1] t[3]
 *
 */

template <typename T>
class matrix2
{
  public:
	T t[4];

	matrix2();
  explicit
	matrix2(T s);
	matrix2(
		T a00, T a10,
		T a01, T a11
	);
  template <typename S>
  	matrix2(matrix2<S> const& m);

	matrix2& operator += (matrix2 const& m);
	matrix2& operator -= (matrix2 const& m);
	matrix2& operator *= (matrix2 const& m);
	matrix2& operator *= (T s);
	matrix2& operator /= (T s);
	matrix2  operator -  () const;

	T det() const;
	void transpose();
	matrix2 transposed() const;
	void invert();
	matrix2 inverse() const;

	tabProxy<T>       operator [] (int col);
	tabProxy<T const> operator [] (int col) const;
};

typedef matrix2<int>    int2x2;
typedef matrix2<float>  float2x2;
typedef matrix2<double> double2x2;

template <typename T>
inline matrix2<T>
operator + (matrix2<T> const& a, matrix2<T> const& b)
{
	return matrix2<T>(
		a.t[0] + b.t[0], a.t[2] + b.t[2],
		a.t[1] + b.t[1], a.t[3] + b.t[3]
	);
}

template <typename T, typename S>
inline matrix2<T>
operator + (matrix2<T> const& a, matrix2<S> const& b)
{
	return a + static_cast<matrix2<T> >(b);
}

template <typename T>
inline matrix2<T>
operator - (matrix2<T> const& a, matrix2<T> const& b)
{
	return matrix2<T>(
		a.t[0] - b.t[0], a.t[2] - b.t[2],
		a.t[1] - b.t[1], a.t[3] - b.t[3]
	);
}

template <typename T, typename S>
inline matrix2<T>
operator - (matrix2<T> const& a, matrix2<S> const& b)
{
	return a - static_cast<matrix2<T> >(b);
}

template <typename T>
inline matrix2<T>
operator * (matrix2<T> const& a, matrix2<T> const& b)
{
	return matrix2<T>(
		a.t[0] * b.t[0] + a.t[2] * b.t[1], a.t[0] * b.t[2] + a.t[2] * b.t[3],
		a.t[1] * b.t[0] + a.t[3] * b.t[1], a.t[1] * b.t[2] + a.t[3] * b.t[3]
	);
}

template <typename T, typename S>
inline matrix2<T>
operator * (matrix2<T> const& a, matrix2<S> const& b)
{
	return a * static_cast<matrix2<T> >(b);
}

template <typename T, typename S>
inline matrix2<T>
operator * (matrix2<T> const& m, S s)
{
	T const cs = static_cast<T>(s);
	return matrix2<T>(
		m.t[0] * cs, m.t[2] * cs,
		m.t[1] * cs, m.t[3] * cs
	);
}

template <typename T, typename S>
inline matrix2<T>
operator * (S s, matrix2<T> const& m)
{
	return m * s;
}

template <typename T, typename S>
inline matrix2<T>
operator / (matrix2<T> const& m, S s)
{
	T const cs = static_cast<T>(s);
	return matrix2<T>(
		m.t[0] / cs, m.t[2] / cs,
		m.t[1] / cs, m.t[3] / cs
	);
}

template <typename T>
inline
matrix2<T>::matrix2()
{/* Do nothing. */}

template <typename T>
inline
matrix2<T>::matrix2(T s)
{
	t[0] = t[1] = t[2] = t[3] = s;
}

template <typename T>
inline
matrix2<T>::matrix2(
	T a00, T a10,
	T a01, T a11
)
{
	t[0] = a00; t[2] = a10;
	t[1] = a01; t[3] = a11;
}

template <typename T>
template <typename S>
inline
matrix2<T>::matrix2(matrix2<S> const& m)
{
	t[0] = static_cast<T>(m.t[0]);
	t[1] = static_cast<T>(m.t[1]);
	t[2] = static_cast<T>(m.t[2]);
	t[3] = static_cast<T>(m.t[3]);
}

template <typename T>
inline matrix2<T>&
matrix2<T>::operator += (matrix2<T> const& m)
{
	t[0] += m.t[0];
	t[1] += m.t[1];
	t[2] += m.t[2];
	t[3] += m.t[3];
	return *this;
}

template <typename T>
inline matrix2<T>&
matrix2<T>::operator -= (matrix2<T> const& m)
{
	t[0] -= m.t[0];
	t[1] -= m.t[1];
	t[2] -= m.t[2];
	t[3] -= m.t[3];
	return *this;
}

template <typename T>
inline matrix2<T>&
matrix2<T>::operator *= (matrix2<T> const& m)
{
	T a00 = t[0] * m.t[0] + t[2] * m.t[1];
	T a01 = t[1] * m.t[0] + t[3] * m.t[1];
	T a10 = t[0] * m.t[2] + t[2] * m.t[3];
	T a11 = t[1] * m.t[2] + t[3] * m.t[3];
	
	t[0] = a00;
	t[1] = a01;
	t[2] = a10;
	t[3] = a11;
	
	return *this;
}

template <typename T>
inline matrix2<T>&
matrix2<T>::operator *= (T s)
{
	t[0] *= s;
	t[1] *= s;
	t[2] *= s;
	t[3] *= s;
	return *this;
}

template <typename T>
inline matrix2<T>&
matrix2<T>::operator /= (T s)
{
	t[0] /= s;
	t[1] /= s;
	t[2] /= s;
	t[3] /= s;
	return *this;
}

template <typename T>
inline T
matrix2<T>::det() const
{
	return det(
		t[0], t[2],
		t[1], t[3]
	);
}

template <typename T>
inline void
matrix2<T>::invert()
{
	operator * (static_cast<T>(1.0) / det());
	std::swap(t[0], t[3]);
	t[1] = -t[1];
	t[2] = -t[2];
}

template <typename T>
inline matrix2<T>
matrix2<T>::inverse() const
{
	T d = static_cast<T>(1.0) / det();
	return matrix2<T>(
		 t[3] * d, -t[2] * d,
		-t[1] * d,  t[0] * d
	);
}

template <typename T>
inline void
matrix2<T>::transpose()
{
	std::swap(t[1], t[2]);
}

template <typename T>
inline matrix2<T>
matrix2<T>::transposed() const
{
	return matrix2<T>(
		t[0], t[1],
		t[2], t[3]
	);
}

template <typename T>
inline matrix2<T>
matrix2<T>::operator - () const
{
	return matrix2<T>(
		-t[0], -t[2],
		-t[1], -t[3]
	);
}

template <typename T>
inline std::ostream&
operator << (std::ostream& ostr, matrix2<T> const& m)
{
	return ostr << '/'  << m.t[0] << ' ' << m.t[2] << "\\\n"
				<< '\\' << m.t[1] << ' ' << m.t[3] << '/';
}

template <typename T>
inline std::istream&
operator >> (std::istream& istr, matrix2<T>& m)
{
	return istr >> m.t[0] >> m.t[2]
				>> m.t[1] >> m.t[3];
}

template <typename T>
inline tabProxy<T>
matrix2<T>::operator [] (int col)
{
	return tabProxy<T>(t + 2 * col);
}

template <typename T>
inline tabProxy<T const>
matrix2<T>::operator [] (int col) const
{
	return tabProxy<T const>(t + 2 * col);
}

} /* Namespace math. */
} /* Namescpace jg. */

#endif /* JG_MATH_MATRIX2_HPP */
