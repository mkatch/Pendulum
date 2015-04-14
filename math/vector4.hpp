#ifndef JG_MATH_VECTOR4_HPP
#define JG_MATH_VECTOR4_HPP

#include <iostream>
#include <cmath>

namespace jg
{
namespace math
{

template <typename T>
class vector4
{
  public:
  	union
	{
		struct
		{
			T x;
			T y;
			T z;
			T w;
		};
		struct
		{
			T r;
			T g;
			T b;
			T a;
		};
		T co[3];
	};

	vector4();
  explicit
	vector4(T _s);
	vector4(T _x, T _y, T _z, T _w);
  template <typename S>
  	vector4(vector4<S> const& _v);

  	operator T*          ();
	operator T const*    () const;
	operator void*       ();
	operator void const* () const;

	vector4& operator += (vector4 const& v);
	vector4& operator -= (vector4 const& v);
	vector4& operator *= (T s);
	vector4& operator /= (T s);
	vector4  operator -  () const;

	T lengthSq() const;
	T length() const;
	void normalize();
	vector4 normalized() const;
	void limit(T l);
	vector4 limited(T l) const;

	inline T operator [] (int i);
};

typedef vector4<int>    int4;
typedef vector4<float>  float4;
typedef vector4<double> double4;

template <typename T>
inline
vector4<T>::operator T* ()
{
	return co;
}

template <typename T>
inline
vector4<T>::operator T const* () const
{
	return co;
}

template <typename T>
inline
vector4<T>::operator void* ()
{
	return co;
}

template <typename T>
inline
vector4<T>::operator void const* () const
{
	return co;
}

template <typename T>
inline vector4<T>
operator + (vector4<T> const& a, vector4<T> const& b)
{
	return vector4<T>(
		a.x + b.x,
		a.y + b.y,
		a.z + b.z,
		a.w + b.w
	);
}

template <typename T, typename S>
inline vector4<T>
operator + (vector4<T> const& a, vector4<S> const& b)
{
	return a + static_cast<vector4<T> >(b);
}

template <typename T>
inline vector4<T>
operator - (vector4<T> const& a, vector4<T> const& b)
{
	return vector4<T>(
		a.x - b.x,
		a.y - b.y,
		a.z - b.z,
		a.w - b.w
	);
}

template <typename T, typename S>
inline vector4<T>
operator - (vector4<T> const& a, vector4<S> const& b)
{
	return a - static_cast<vector4<T> >(b);
}

template <typename T, typename S>
inline vector4<T>
operator * (vector4<T> const& v, S s)
{
	T const cs = static_cast<T>(s);
	return vector4<T>(
		v.x * cs,
		v.y * cs,
		v.z * cs,
		v.w * cs
	);
}

template <typename T, typename S>
inline vector4<T>
operator * (S s, vector4<T> const& v)
{
	return v * s;
}

template <typename T, typename S>
inline vector4<T>
operator / (vector4<T> const& v, S s)
{
	T const cs = static_cast<T>(s);
	return vector4<T>(
		v.x / cs,
		v.y / cs,
		v.z / cs,
		v.w / cs);
}

template <typename T>
inline T
dot(vector4<T> const& a, vector4<T> const& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

template <typename T, typename S>
inline T
dot(vector4<T> const& a, vector4<S> const& b)
{
	return dot(a, static_cast<vector4<T> >(b));
}

template <typename T>
vector4<T>::vector4()
{/* Do nothing. */}

template <typename T>
vector4<T>::vector4(T _s)
: x(_s), y(_s), z(_s), w(_s)
{/* Do nothing. */}

template <typename T>
vector4<T>::vector4(T _x, T _y, T _z, T _w)
: x(_x), y(_y), z(_z), w(_w)
{/* Do nothing. */}

template <typename T>
template <typename S>
vector4<T>::vector4(vector4<S> const& _v)
: x(_v.x), y(_v.y), z(_v.z), w(_v.w)
{/* Do nothing. */}

template <typename T>
inline vector4<T>&
vector4<T>::operator += (vector4<T> const& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

template <typename T>
inline vector4<T>&
vector4<T>::operator -= (vector4<T> const& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

template <typename T>
inline vector4<T>&
vector4<T>::operator *= (T s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

template <typename T>
inline vector4<T>&
vector4<T>::operator /= (T s)
{
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}

template <typename T>
inline vector4<T>
vector4<T>::operator - () const
{
	return vector4<T>(-x, -y, -z, -w);
}

template <typename T>
inline T
vector4<T>::lengthSq() const
{
	return x * x + y * y + z * z + w * w;
}

template <typename T>
inline T
vector4<T>::length() const
{
	return std::sqrt(lengthSq());
}

template <typename T>
inline void
vector4<T>::normalize()
{
	return operator /= (length());
}

template <typename T>
inline vector4<T>
vector4<T>::normalized() const
{
	return *this / length();
}

template <typename T>
inline void
vector4<T>::limit(T l)
{
	l /= length();
	if(l > 1) return operator *= (l);
	else if(l < 0 && l > -1) return operator *= (-l);
}

template <typename T>
inline vector4<T>
vector4<T>::limited(T l) const
{
	l /= length();
	if(l > 1) return *this * l;
	else if(l < 0 && l > -1) return *this * (-l);
}

template <typename T>
inline T
vector4<T>::operator [] (int i)
{
	return co[i];
}

template <typename T>
inline std::ostream&
operator << (std::ostream& ostr, vector4<T> const& v)
{
	return ostr << '[' << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w << ']'; 
}


template <typename T>
inline std::istream&
operator >> (std::istream& istr, vector4<T>& v)
{
	return istr >> v.x >> v.y >> v.z >> v.w;
}

} /* Namespace math. */
} /* Namespace jg. */

#endif /* JG_MATH_VECTOR4_HPP */
