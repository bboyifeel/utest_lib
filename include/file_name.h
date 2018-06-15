#pragma once

// All characters will be converted to lower case
// All symbols '\' will be replaced with '/'
namespace xIO
{

class FileName
{
public:
	FileName() {}
	FileName(const std::string& s)	{ Set(s); }
	FileName(const FileName& n)	{ Set(n); }
	FileName(const char* s)			{ Set(s); }

	FileName&		operator=(const char* s);
	FileName&		operator=(const std::string& s);
	FileName&		operator=(const FileName& filename);

	const std::string&	str()	const				{ return data;			}
	const char*			c_str()	const				{ return data.c_str();	}
	bool				empty()	const				{ return data.empty();	}
	size_t				size()	const				{ return data.size();	}
	void				clear()						{ data.erase();			}

	FileName&			Path();
	bool				IsPath() const;

	void				Add(const std::string& s);
	FileName			GetAdd(const FileName& n)	const;
private:
	virtual char		DelimiterWin()				const	{ return '\\';		}
	virtual char		Delimiter()					const	{ return '/';		}
	void				Set(const std::string& s);
	void				Set(const char* s);
	void				Set(const FileName& fileName);
	void				Validate();

	std::string			data;
};

}//namespace xIO