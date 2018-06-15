#pragma once
#include <functional>
#include "utils.h"

template <class T> struct SameValue
{
	SameValue(T _value) : value(_value) {};
	bool operator()(T sample) { return sample == value; }
	T value;
};

template<class T> class eVectorItemMultiplier_Default
{
public:
	static T*	Copy(T* _item)
	{
		//LSD_STOP;
		return _item;
	}
};

template<class T> class eVectorItemComparer_Simple
{
public:
	static bool	Equal(const T* _item1, const T* _item2)
	{
		return _item1 == _item2;
	}
};

template<class T
	, class Copier = eVectorItemMultiplier_Default<T>
	, class Comparer = eVectorItemComparer_Simple<T>
>
class eVectorBase : protected std::vector<T*>
{
	using eInherited = std::vector<T*>;
public:
	eVectorBase() = default;
	eVectorBase(const eVectorBase& _exmp) { Assign(_exmp); }
	eVectorBase(eVectorBase&&) = default;
	eVectorBase(std::initializer_list<T*> _initList) : eInherited(_initList) {}
	virtual ~eVectorBase() { /*LSD_DESIRE(empty());*/ }

	using iterator = typename eVectorBase::iterator;
	using const_iterator = typename eVectorBase::const_iterator;
	using reverse_iterator = typename eVectorBase::reverse_iterator;
	using const_reverse_iterator = typename eVectorBase::const_reverse_iterator;
	using size_type = typename eVectorBase::size_type;
	using value_type = typename eVectorBase::value_type;

	eVectorBase& operator=(const eVectorBase& _exmp)
	{
		Assign(_exmp);
		return *this;
	}
	eVectorBase& operator=(eVectorBase&&) = default;
	bool operator==(const eVectorBase& _exmp) const
	{
		if (size() != _exmp.size()) return false;

		for (size_type i = size(); i-- > 0;)
		{
			if (!Comparer::Equal((*this)[i], _exmp[i]))
			{
				return false;
			}
		}
		return true;
	}
	bool operator!=(const eVectorBase& _exmp) const
	{
		return !operator==(_exmp);
	}
	virtual void Clear()
	{
		if (empty()) { return; }

		ForEach(eVectorBase, i, *this)
		{
			DestroyItem(*i);
		}
		this->clear();
	}
	virtual void Add(T* _item)
	{
		this->push_back(_item);
	}
	virtual void AddFront(T* _item)
	{
		this->insert(begin(), _item);
	}
	virtual bool Delete(T* _item)
	{
		iterator it = std::find(begin(), end(), _item);
		if (it != end())
		{
			DestroyItem(_item);
			this->erase(it);
			return true;
		}
		return false;
	}
	virtual void CutTo(size_type _indFrom, size_type _count, eVectorBase& _to)
	{
		if (!empty() && _indFrom < size())
		{
			size_type	count = Min<size_type>(_count > 0 ? _count : size(), size() - _indFrom);
			iterator	from = begin() + _indFrom;
			iterator	to = from + count;

			_to.reserve(_to.size() + count);
			_to.insert(_to.end(), from, to);
			this->erase(from, to);
		}
	}
	virtual void CutTotalTo(eVectorBase& _to)
	{
		CutTo(0, size(), _to);
	}
	virtual void DeleteIf(const std::function<bool(T*)>& _comparer)
	{
		iterator from = remove_if(begin(), end(), [this, &_comparer](T* _item)
		{
			if (_comparer(_item))
			{
				DestroyItem(_item);
				return true;
			}
			return false;
		});
		this->erase(from, end());
	}
	virtual bool DeleteSingleIf(const std::function<bool(T*)>& _comparer)
	{
		iterator it = find_if(begin(), end(), [this, &_comparer](T* _item)
		{
			if (_comparer(_item))
			{
				DestroyItem(_item);
				return true;
			}
			return false;
		});
		if (it != end())
		{
			this->erase(it);
			return true;
		}
		return false;
	}
	virtual void CutToIf(eVectorBase& _to, const std::function<bool(T*)>& _comparer)
	{
		iterator	from = stable_partition(begin(), end(), not1(_comparer));
		size_type	count = distance(from, end());

		_to.reserve(_to.size() + count);
		_to.insert(_to.end(), from, end());
		this->erase(from, end());
	}
	virtual void CopyIf(eVectorBase& _to, const std::function<bool(T*)>& _comparer)
	{
		size_type count = 0;
		_to.reserve(size());
		copy_if(begin(),
			end(),
			back_inserter(static_cast<eInherited&>(_to)),
			[this, &count, _comparer](T* _item)
		{
			if (_comparer(_item))
			{
				PrepareToCopy(_item);
				++count;
				return true;
			}
			return false;
		});
		_to.resize(count);
	}
	virtual T*	FindIf(const std::function<bool(T*)>& _comparer)
	{
		T* ret = nullptr;
		const_iterator it = find_if(begin(), end(), _comparer);
		if (it != end())
		{
			ret = *it;
		}
		return ret;
	}
	virtual const T* FindIf(const std::function<bool(const T*)>& _comparer) const
	{
		T* ret = nullptr;
		const_iterator it = find_if(begin(), end(), _comparer);
		if (it != end())
		{
			ret = *it;
		}
		return ret;
	}

	// safe access (without erase or delete, or something like that)
	size_type				size()			  const { return eInherited::size(); }
	bool					empty()			  const { return eInherited::empty(); }
	size_type				capacity()		  const { return eInherited::capacity(); }
	void					reserve(size_type n) { eInherited::reserve(n); }

	iterator				begin() { return eInherited::begin(); }
	const_iterator			begin()			  const { return eInherited::begin(); }
	iterator				end() { return eInherited::end(); }
	const_iterator			end()			  const { return eInherited::end(); }
	reverse_iterator		rbegin() { return eInherited::rbegin(); }
	const_reverse_iterator	rbegin()		  const { return eInherited::rbegin(); }
	reverse_iterator		rend() { return eInherited::rend(); }
	const_reverse_iterator	rend()			  const { return eInherited::rend(); }
	T&						at(size_type ind) const { /*LSD_DESIRE(ind >= 0 && ind < size());*/ return (T&)*(*this)[ind]; }
	T&						front() { /*LSD_DESIRE(size() > 0); */return *eInherited::front(); }
	const T&				front()			  const { /*LSD_DESIRE(size() > 0);*/ return *eInherited::front(); }

	void delete_for(SameValue<T*> _condition)
	{
        ::delete_for((std::vector<T*>&)*this, _condition);
	}

protected:
	virtual void			DestroyItem(T*& item) = 0;

	void Assign(const eVectorBase& _exmp)
	{
		Clear();
		CForEach(eVectorBase, i, _exmp)
		{
			this->push_back(Copier::Copy(*i));	// Don't use Add
		}
	}
	virtual void PrepareToCut(T*& _item) {}
	virtual void PrepareToCopy(T*& _item) {}
};

template<class T
	, class Copier = eVectorItemMultiplier_Default<T>
	, class Comparer = eVectorItemComparer_Simple<T>
>
class eVectorGeneric : public eVectorBase<T, Copier, Comparer>
{
	using eInherited = eVectorBase<T, Copier, Comparer>;
public:
	eVectorGeneric() = default;
	eVectorGeneric(const eVectorGeneric& _exmp) : eInherited(_exmp) {}
	eVectorGeneric(eVectorGeneric&&) = default;
	eVectorGeneric(std::initializer_list<T*> _initList) : eInherited(_initList) {}
	virtual ~eVectorGeneric() = default;

	eVectorGeneric&	operator=(const eVectorGeneric&) = default;
	eVectorGeneric&	operator=(eVectorGeneric&&) = default;

protected:
	virtual void DestroyItem(T*& _item) override
	{
		::SAFE_DELETE(_item);
	}
};
