#pragma once

template<class T, class D = T> class Entry
{
public:
	class eDef
	{
		friend class Entry;
		static Entry*& Get()
		{
			static Entry* f = NULL; return f;
		}
	};
	Entry(const char* _name, const T& _info) : caption(_name), info(_info), next(NULL)
	{
		if(!First())
		{
			eDef::Get() = this;
		}
		else
		{
			Entry* entry = First();
			for(; entry->next; entry = entry->next);
			entry->next = this;
		}
	}

	const char*		Name() const
	{
		return caption;
	}
	T&				Info()
	{
		return info;
	}
	const T&		Info() const
	{
		return info;
	}

	static Entry*	First()
	{
		return eDef::Get();
	}
	Entry*			Next() const
	{
		return next;
	}
	static Entry*	Find(const char* c)
	{
		return Find(c, First());
	}
	static Entry*	Get(const std::string& c)
	{
		return Find(c.c_str());
	}
	static Entry*	Get(const char* c)
	{
		return Find(c);
	}

protected:
	static Entry*	Find(const char* _name, Entry* _start)
	{
		for(Entry* entry = _start; entry; entry = entry->next)
		{
			if(!xBase::strcasecompare(_name, entry->caption))
			{
				return entry;
			}
		}
		return NULL;
	}

protected:
	const char*	caption;
	T			info;
	Entry*		next;
};