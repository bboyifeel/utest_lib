#pragma once

#include <cstring>
#include <iterator>
#include <algorithm>

#ifndef WINDOWS
#   include <unistd.h>
#endif

#define	SAFE_CALL(p)			if(p) (p)
#define CAST(cls, expr)			(dynamic_cast<cls*>(expr))

template<class T> inline void SAFE_DELETE(T*& p)
{
	if (p) { delete p; p = NULL; }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEFINE_POST_STATE(type, vname, fname)											\
public:																					\
inline	type fname() const { return (type)vname; };										\
inline	void fname(type _state)															\
{																						\
	if (vname != _state)																\
	{																					\
		vname = _state;																	\
		On##fname();																	\
	}																					\
};																						\
protected:																				\
virtual	void On##fname();																\
private:

////////////////////////////////////////////////////////////////////////////////////////////////////
#define DEFINE_SIMPLE_STATE(type, vname, fname)											\
public:																					\
inline	type fname() const { return (type)vname; }										\
inline	void fname(type _state) { vname = _state; }										\
private:

#define ForEach(iClass, i, list)															\
if(!(list).empty())																			\
	for(iClass::iterator i = (list).begin(), e_##i = (list).end() ; i != e_##i; ++i)		\

#define CForEach(iClass, i, list)															\
if(!(list).empty())																			\
	for(iClass::const_iterator i = (list).begin(), e_##i = (list).end(); i != e_##i; ++i)	\


inline void						Clear(void* a, size_t size)	{ memset(a, 0, size); 	}
template <class T> inline void	Clear(T& a)					{ Clear(&a, sizeof(a)); }


#ifdef WINDOWS

inline int32_t		strcasecompare(const char* _str1, const char* _str2)						{ return _stricmp(_str1, _str2); }

#else //WINDOWS

inline int32_t		strcasecompare(const char* _str1, const char* _str2)						{ return strcasecmp(_str1, _str2); }

#endif //WINDOWS

template <class T>
inline const T& Min(const T& x, const T& y)
{
    return (x <= y) ? x : y;
}

template <class T, class _Pr>
size_t delete_for(std::vector<T>& _vector, _Pr _condition)
{
    typename std::vector<T>::iterator  from  = std::remove_if(_vector.begin(), _vector.end(), _condition);
    size_t        count  = std::distance(from, _vector.end());
    _vector.erase(from, _vector.end());
    return count;
}

#ifndef WINDOWS
inline void    Sleep(uint32_t ms)          { usleep(useconds_t(ms) * 1000); }
#endif
