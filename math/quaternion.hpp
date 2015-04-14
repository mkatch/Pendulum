#ifndef JG_MATH_QUATERNION_HPP
#define JG_MATH_QUATERNION_HPP

#include <cmath>
#include "vector3.hpp"
#include "matrix4.hpp"

namespace jg
{
namespace math
{

template <typename T>
class quaternion
{
  public:
	T          s;
	vector3<T> v;

	quaternion();
	quaternion(T _s, vector3<T> const& _v);
	quaternion(T _w, T _x, T _y, T _z);
  template <typename S>
  	quaternion(quaternion<S> const& q);

	T          lengthSq()   const;
	T          length()     const;
	quaternion conjugate()  const;
	quaternion inverse()    const;
	quaternion normalized() const;
	void       normalize();
	void       invert();
	matrix4<T> matrix()     const;

	quaternion& operator += (quaternion<T> const& q);
	quaternion& operator -= (quaternion<T> const& q);
	quaternion& operator *= (quaternion<T> const& q);
	quaternion& operator *= (T s);
	quaternion& operator /= (T s);
	quaternion  operator  - () const;

  private:
  	static T          const S_DEFAULT;
	static vector3<T> const V_DEFAULT;
};

typedef quaternion<float> qfloat;
typedef quaternion<double> qdouble;

template <typename T>
T const quaternion<T>::S_DEFAULT = 0;
template <typename T>
vector3<T> const quaternion<T>::V_DEFAULT = 0;

template <typename T>
inline
quaternion<T>::quaternion()
: s(S_DEFAULT), v(V_DEFAULT)
{/* DO nothing. */}

template <typename T>
inline
quaternion<T>::quaternion(T _s, vector3<T> const& _v)
: s(_s), v(_v)
{/* Do nothing. */}

template <typename T>
inline
quaternion<T>::quaternion(T _w, T _x, T _y, T _z)
: s(_w), v(_x, _y, _z)
{/* Do nothing. */}

template <typename T>
template <typename S>
inline
quaternion<T>::quaternion(quaternion<S> const& q)
: s(q.s), v(q.v)
{/* Do nothing. */}

template <typename T>
inline T
quaternion<T>::lengthSq() const
{
	return s * s + v.lengthSq();
}

template <typename T>
inline T
quaternion<T>::length() const
{	
	return std::sqrt(lengthSq());
}

template <typename T>
inline quaternion<T>
quaternion<T>::conjugate() const
{
	return quaternion<T>(s, -v);
}

template <typename T>
inline quaternion<T>
quaternion<T>::inverse() const
{
	return conjugate() / lengthSq();
}

template <typename T>
inline quaternion<T>
quaternion<T>::normalized() const
{
	return *this / length();
}

template <typename T>
inline void
quaternion<T>::normalize()
{
	operator /= (length());
}

template <typename T>
inline void
quaternion<T>::invert()
{
	*this = inverse();
}

template <typename T>
inline matrix4<T>
quaternion<T>::matrix() const
{
	T const w = s;
	T const x = v.x;
	T const y = v.y;
	T const z = v.z;
	T const d = static_cast<T>(2);

	return matrix4<T>(
		w*w+x*x-y*y-z*z,   d*(x*y-z*w)  ,   d*(x*z+y*w)  ,        0       ,
		  d*(x*y+z*w)  , w*w-x*x+y*y-z*z,   d*(y*z-x*w)  ,        0       ,
		  d*(x*z-y*w)  ,   d*(y*z+x*w)  , w*w-x*x-y*y+z*z,        0       ,
	           0       ,        0       ,         0      , w*w+x*x+y*y+z*z
	);
}

template <typename T>
inline quaternion<T>&
quaternion<T>::operator += (quaternion<T> const& q)
{
	s += q.s;
	v += q.v;
	return *this;
}

template <typename T>
inline quaternion<T>&
quaternion<T>::operator -= (quaternion<T> const& q)
{
	return operator += (-q);
}

template <typename T>
inline quaternion<T>&
quaternion<T>::operator *= (quaternion<T> const& q)
{
	T          newS = s * q.s - dot(v, q.v);
	vector3<T> newV = s * q.v + q.s * v + cross(v, q.v);
	s = newS;
	v = newV;
	return *this;
}

template <typename T>
inline quaternion<T>&
quaternion<T>::operator *= (T n)
{
	s *= n;
	v *= n;
	return *this;
}

template <typename T>
inline quaternion<T>&
quaternion<T>::operator /= (T n)
{
	return operator *= (static_cast<T>(1) / n);
}

template <typename T>
inline quaternion<T>
quaternion<T>::operator - () const
{
	return quaternion(-s, -v);
}

template <typename T, typename S>
inline quaternion<T>
operator + (quaternion<T> p, quaternion<S> const& q)
{
	return p += q;
}

template <typename T, typename S>
inline quaternion<T>
operator - (quaternion<T> p, quaternion<S> const& q)
{
	return p -= q;
}

template <typename T, typename S>
inline quaternion<T>
operator * (quaternion<T> p, quaternion<S> const& q)
{
	return p *= q;
}

template <typename T, typename S>
inline quaternion<T>
operator * (quaternion<T> p, S n)
{
	return p *= n;
}

template <typename T, typename S>
inline quaternion<T>
operator * (S n, quaternion<T> p)
{
	return p *= n;
}

template <typename T, typename S>
inline quaternion<T>
operator / (quaternion<T> p, S n)
{
	return p /= n;
}

} /* Namespace math. */
} /* Namespace jg. */
	
#endif
