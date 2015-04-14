#ifndef JG_TABPROXY_HPP
#define JG_TABPROXY_HPP

namespace jg {

template <typename T>
class tabProxy
{
  private:
  	T* const my_v;

  public:
	T& operator [] (int i);
	tabProxy(T* v);
};

template <typename T>
inline
tabProxy<T>::tabProxy(T* v)
: my_v(v)
{/* Do nothing. */}

template <typename T>
inline
T& tabProxy<T>::operator [] (int i)
{
	return my_v[i];
}

} /* Namespace jg. */

#endif /* JG_TABPROXY_HPP */
