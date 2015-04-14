#ifndef JG_MATH_VECTOR2_HPP
#define JG_MATH_VECTOR2_HPP

#include <iostream>
#include <cmath>

namespace jg
{
namespace math
{

template <typename T>
class vector2
{
  public:
  	union
	{
		struct
		{
			T x;
			T y;
		};
		struct
		{
			T u;
			T v;
		};
		T co[2];
	};

	vector2();
  explicit
	vector2(T _s);
	vector2(T _x, T _y);
  template <typename S>
  	vector2(vector2<S> const& _v);

  	operator T*          ();
	operator T const*    () const;
	operator void*       ();
	operator void const* () const;

	vector2& operator += (vector2 const& v);
	vector2& operator -= (vector2 const& v);
	vector2& operator *= (T s);
	vector2& operator /= (T s);
	vector2  operator -  () const;

	T lengthSq() const;
	T length() const;
	void normalize();
	vector2 normalized() const;
	void limit(T l);
	vector2 limited(T l) const;

	T operator [] (int i);
};

typedef vector2<int>    int2;
typedef vector2<float>  float2;
typedef vector2<double> double2;

template <typename T>
inline
vector2<T>::operator T* ()
{
	return co;
}

template <typename T>
inline
vector2<T>::operator T const* () const
{
	return co;
}

template <typename T>
inline
vector2<T>::operator void* ()
{
	return co;
}

template <typename T>
inline
vector2<T>::operator void const* () const
{
	return co;
}

template <typename T>
inline vector2<T>
operator + (vector2<T> const& a, vector2<T> const& b)
{
	return vector2<T>(a.x + b.x, a.y + b.y);
}

template <typename T, typename S>
inline vector2<T>
operator + (vector2<T> const& a, vector2<S> const& b)
{
	return a + static_cast<vector2<T> >(b);
}

template <typename T>
inline vector2<T>
operator - (vector2<T> const& a, vector2<T> const& b)
{
	return vector2<T>(
		a.x - b.x,
		a.y - b.y
	);
}

template <typename T, typename S>
inline vector2<T>
operator - (vector2<T> const& a, vector2<S> const& b)
{
	return a - static_cast<vector2<T> >(b);
}

template <typename T, typename S>
inline vector2<T>
operator * (vector2<T> const& v, S s)
{
	return vector2<T>(v.x * static_cast<T>(s),
		v.y * static_cast<T>(s));
}

template <typename T, typename S>
inline vector2<T>
operator * (S s, vector2<T> const& v)
{
	return v * s;
}

template <typename T, typename S>
inline vector2<T>
operator / (vector2<T> const& v, S s)
{
	return vector2<T>(
		v.x / static_cast<T>(s),
		v.y / static_cast<T>(s)
	);
}

template <typename T>
inline T
dot(vector2<T> const& a, vector2<T> const& b)
{
	return a.x * b.x + a.y * b.y;
}

template <typename T, typename S>
inline T
dot(vector2<T> const& a, vector2<S> const& b)
{
	return dot(a, static_cast<vector2<T> >(b));
}

template <typename T>
inline T
cross(vector2<T> const& a, vector2<T> const& b)
{
	return a.x * b.y - a.y * b.x;
}

template <typename T, typename S>
inline T
cross(vector2<T> const& a, vector2<S> const& b)
{
	return cross(a, static_cast<vector2<T> >(b));
}

template <typename T>
vector2<T>::vector2()
{/* Do nothing. */}

template <typename T>
vector2<T>::vector2(T _s)
: x(_s), y(_s)
{/* Do nothing. */}

template <typename T>
vector2<T>::vector2(T _x, T _y)
: x(_x), y(_y)
{/* Do nothing. */}

template <typename T>
template <typename S>
vector2<T>::vector2(vector2<S> const& _v)
: x(_v.x), y(_v.y)
{/* Do nothing. */}

template <typename T>
inline vector2<T>&
vector2<T>::operator += (vector2<T> const& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

template <typename T>
inline vector2<T>&
vector2<T>::operator -= (vector2<T> const& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

template <typename T>
inline vector2<T>&
vector2<T>::operator *= (T s)
{
	x *= s;
	y *= s;
	return *this;
}

template <typename T>
inline vector2<T>&
vector2<T>::operator /= (T s)
{
	x /= s;
	y /= s;
	return *this;
}

template <typename T>
inline vector2<T>
vector2<T>::operator - () const
{
	return vector2<T>(-x, -y);
}

template <typename T>
inline T
vector2<T>::lengthSq() const
{
	return x * x + y * y;
}

template <typename T>
inline T
vector2<T>::length() const
{
	return std::sqrt(lengthSq());
}

template <typename T>
inline void
vector2<T>::normalize()
{
	return operator /= (length());
}

template <typename T>
inline vector2<T>
vector2<T>::normalized() const
{
	return *this / length();
}

template <typename T>
inline void
vector2<T>::limit(T l)
{
	l /= length();
	if(l > 1) return operator *= (l);
	else if(l < 0 && l > -1) return operator *= (-l);
}

template <typename T>
inline vector2<T>
vector2<T>::limited(T l) const
{
	l /= length();
	if(l > 1) return *this * l;
	else if(l < 0 && l > -1) return *this * (-l);
}

template <typename T>
inline T
vector2<T>::operator [] (int i)
{
	return co[i];
}

template <typename T>
inline std::ostream&
operator << (std::ostream& ostr, vector2<T> const& v)
{
	return ostr << "[ " << v.x << ' ' << v.y << " ]"; 
}

template <typename T>
inline std::istream&
operator >> (std::istream& istr, vector2<T>& v)
{
	return istr >> v.x >> v.y;
}

} /* Namespace math. */
} /* Namespace jg. */

#endif /* JG_MATH_VECTOR2_HPP */
