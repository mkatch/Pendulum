#ifndef JG_MATH_VECTOR_HPP
#define JG_MATH_VECTOR_HPP

#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"

#include <vector>

// TODO

template <typename T> inline
std::vector<T>& operator += (std::vector<T>& a, std::vector<T> const& b)
{
	for (int i = 0; i < a.size(); ++i) a[i] += b[i];
	return a;
}

template <typename T> inline
std::vector<T>& operator -= (std::vector<T>& a, std::vector<T> const& b)
{
	for (int i = 0; i < a.size(); ++i) a[i] -= b[i];
	return a;
}

template <typename T> inline
std::vector<T>& operator *= (std::vector<T>& v, T s)
{
	for (int i = 0; i < v.size(); ++i) v[i] *= s;
	return v;
}

template <typename T, typename S> inline
std::vector<T>& operator *= (std::vector<T>& v, S s)
{
	return v *= static_cast<T>(s);
}

template <typename T> inline
std::vector<T>& operator /= (std::vector<T>& v, T s)
{
	for (int i = 0; i < v.size(); ++i) v[i] /= s;
	return v;
}

template <typename T, typename S> inline
std::vector<T>& operator /= (std::vector<T>& v, S s)
{
	return v /= static_cast<T>(s);
}

template <typename T> inline
std::vector<T> operator - (std::vector<T> v)
{
	for (int i = 0; i < v.size(); ++i) v[i] = -v[i];
	return v;
}

template <typename T> inline
std::vector<T> operator + (std::vector<T> a, std::vector<T> const& b)
{
	return a += b;
}

template <typename T> inline
std::vector<T> operator - (std::vector<T> a, std::vector<T> const& b)
{
	return a -= b;
}

template <typename T, typename S> inline
std::vector<T> operator * (S s, std::vector<T> v)
{
	return v *= s;
}

template <typename T, typename S> inline
std::vector<T> operator * (std::vector<T> v, S s)
{
	return v *= s;
}

template <typename T, typename S> inline
std::vector<T> operator / (std::vector<T> v, S s)
{
	return v /= s;
}

#endif /* JG_MATH_VECTOR_HPP */
