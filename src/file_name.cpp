#include "stdafx.h"
#include "file_name.h"

namespace xIO
{

FileName& FileName::operator=(const std::string& s)
{
	Set(s);
	return *this;
}

FileName& FileName::operator=(const FileName& filename)
{
	Set(filename.data);
	return *this;
}

FileName& FileName::operator=(const char* s)
{
	Set(s);
	return *this;
}

FileName& FileName::Path()
{
	if (!IsPath())
		data += Delimiter();
	return *this;
}

bool FileName::IsPath() const
{
	return !empty() && data[size() - 1] == Delimiter();
}

void FileName::Add(const std::string& s)
{
	data += s;
}

FileName FileName::GetAdd(const FileName& n) const
{
	FileName r = *this;
	r.Add(n.data);
	return r;
}

void FileName::Set(const std::string& s)
{
	data = s;
	Validate();
}

void FileName::Set(const FileName& fileName)
{
	data = fileName.data;
	Validate();
}

void FileName::Set(const char* s)
{
	if (s) { data = s; Validate(); }
	else clear();
}

void FileName::Validate()
{
	replace(data.begin(), data.end(), DelimiterWin(), Delimiter());
}

}//namespace xIO