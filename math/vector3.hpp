#ifndef JG_MATH_VECTOR3_HPP
#define JG_MATH_VECTOR3_HPP

#include <iostream>
#include <cmath>

namespace jg
{
namespace math
{

template <typename T>
class vector3
{
  public:
  	union
	{
		struct
		{
			T x;
			T y;
			T z;
		};
		struct
		{
			T r;
			T g;
			T b;
		};
		struct
		{
			T u;
			T v;
			T w;
		};
		T co[3];
	};

	vector3();
  explicit
	vector3(T _s);
	vector3(T _x, T _y, T _z);
  template <typename S>
  	vector3(vector3<S> const& _v);

  	operator T*          ();
	operator T const*    () const;
	operator void*       ();
	operator void const* () const;

	vector3& operator += (vector3 const& v);
	vector3& operator -= (vector3 const& v);
	vector3& operator *= (T s);
	vector3& operator /= (T s);
	vector3  operator -  () const;

	T lengthSq() const;
	T length() const;
	void normalize();
	vector3 normalized() const;
	void limit(T l);
	vector3 limited(T l) const;

	T operator [] (int i);
};

typedef vector3<int>    int3;
typedef vector3<float>  float3;
typedef vector3<double> double3;

template <typename T>
inline
vector3<T>::operator T* ()
{
	return co;
}

template <typename T>
inline
vector3<T>::operator T const* () const
{
	return co;
}

template <typename T>
inline
vector3<T>::operator void* ()
{
	return co;
}

template <typename T>
inline
vector3<T>::operator void const* () const
{
	return co;
}

template <typename T>
inline vector3<T>
operator + (vector3<T> const& a, vector3<T> const& b)
{
	return vector3<T>(
		a.x + b.x,
		a.y + b.y,
		a.z + b.z
	);
}

template <typename T, typename S>
inline vector3<T>
operator + (vector3<T> const& a, vector3<S> const& b)
{
	return a + static_cast<vector3<T> >(b);
}

template <typename T>
inline vector3<T>
operator - (vector3<T> const& a, vector3<T> const& b)
{
	return vector3<T>(
		a.x - b.x,
		a.y - b.y,
		a.z - b.z
	);
}

template <typename T, typename S>
inline vector3<T>
operator - (vector3<T> const& a, vector3<S> const& b)
{
	return a - static_cast<vector3<T> >(b);
}

template <typename T, typename S>
inline vector3<T>
operator * (vector3<T> const& v, S s)
{
	T const cs = static_cast<T>(s);
	return vector3<T>(
		v.x * cs,
		v.y * cs,
		v.z * cs
	);
}

template <typename T, typename S>
inline vector3<T>
operator * (S s, vector3<T> const& v)
{
	return v * s;
}

template <typename T, typename S>
inline vector3<T>
operator / (vector3<T> const& v, S s)
{
	T const cs = static_cast<T>(s);
	return vector3<T>(
		v.x / cs,
		v.y / cs,
		v.z / cs
	);
}

template <typename T>
inline T
dot(vector3<T> const& a, vector3<T> const& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

template <typename T, typename S>
inline T
dot(vector3<T> const& a, vector3<S> const& b)
{
	return dot(a, static_cast<vector3<T> >(b));
}

template <typename T>
inline vector3<T>
cross(vector3<T> const& a, vector3<T> const& b)
{
	return vector3<T>(
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	);
}

template <typename T, typename S>
inline vector3<T>
cross(vector3<T> const& a, vector3<S> const& b)
{
	return cross(a, static_cast<vector3<T> >(b));
}

template <typename T>
vector3<T>::vector3()
{/* Do nothing. */}

template <typename T>
vector3<T>::vector3(T _s)
: x(_s), y(_s), z(_s)
{/* Do nothing. */}

template <typename T>
vector3<T>::vector3(T _x, T _y, T _z)
: x(_x), y(_y), z(_z)
{/* Do nothing. */}

template <typename T>
template <typename S>
vector3<T>::vector3(vector3<S> const& _v)
: x(_v.x), y(_v.y), z(_v.z)
{/* Do nothing. */}

template <typename T>
inline vector3<T>&
vector3<T>::operator += (vector3<T> const& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

template <typename T>
inline vector3<T>&
vector3<T>::operator -= (vector3<T> const& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

template <typename T>
inline vector3<T>&
vector3<T>::operator *= (T s)
{
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

template <typename T>
inline vector3<T>&
vector3<T>::operator /= (T s)
{
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

template <typename T>
inline vector3<T>
vector3<T>::operator - () const
{
	return vector3<T>(-x, -y, -z);
}

template <typename T>
inline T
vector3<T>::lengthSq() const
{
	return x * x + y * y + z * z;
}

template <typename T>
inline T
vector3<T>::length() const
{
	return std::sqrt(lengthSq());
}

template <typename T>
inline void
vector3<T>::normalize()
{
	return operator /= (length());
}

template <typename T>
inline vector3<T>
vector3<T>::normalized() const
{
	return *this / length();
}

template <typename T>
inline void
vector3<T>::limit(T l)
{
	l /= length();
	if(l > 1) return operator *= (l);
	else if(l < 0 && l > -1) return operator *= (-l);
}

template <typename T>
inline vector3<T>
vector3<T>::limited(T l) const
{
	l /= length();
	if(l > 1) return *this * l;
	else if(l < 0 && l > -1) return *this * (-l);
}

template <typename T>
inline T
vector3<T>::operator [] (int i)
{
	return co[i];
}

template <typename T>
inline std::ostream&
operator << (std::ostream& ostr, vector3<T> const& v)
{
	return ostr << '[' << v.x << ' ' << v.y << ' ' << v.z << ']'; 
}

template <typename T>
inline std::istream&
operator >> (std::istream& istr, vector3<T>& v)
{
	return istr >> v.x >> v.y >> v.z;
}

} /* Namespace math. */
} /* Namespace jg. */

#endif /* JG_MATH_VECTOR3_HPP */
