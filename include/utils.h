#pragma once

#include <cstring>
#include <iterator>
#include <algorithm>

#ifndef WINDOWS
#   include <unistd.h>
#endif

#ifndef WINDOWS
inline void            Sleep(uint32_t ms)          { usleep(useconds_t(ms) * 1000); }
#endif

namespace xBase
{

template<class T> inline void SAFE_DELETE(T*& p)
{
	if (p) { delete p; p = NULL; }
}

template <class T>
inline const T& Min(const T& x, const T& y)
{
	return (x <= y) ? x : y;
}

template <class T, class _Pr>
size_t delete_for(std::vector<T>& _vector, _Pr _condition)
{
	typename std::vector<T>::iterator  from = std::remove_if(_vector.begin(), _vector.end(), _condition);
	size_t        count = std::distance(from, _vector.end());
	_vector.erase(from, _vector.end());
	return count;
}

inline void						Clear(void* a, size_t size)	{ memset(a, 0, size); 	}
template <class T> inline void	Clear(T& a)					{ Clear(&a, sizeof(a)); }

#ifdef WINDOWS
inline int32_t		strcasecompare(const char* _str1, const char* _str2)						{ return _stricmp(_str1, _str2); }
#else //WINDOWS
inline int32_t		strcasecompare(const char* _str1, const char* _str2)						{ return strcasecmp(_str1, _str2); }
#endif //WINDOWS

}//namespace xBase
