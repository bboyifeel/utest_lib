#include "stdafx.h"
#include "file_search.h"

#ifndef WINDOWS
#	include <fnmatch.h>
#endif//WINDOWS

namespace xIO
{

//*****************************************************************************
//	eSearch::Begin
//-----------------------------------------------------------------------------
void Search::Begin(const FileName& _path, const FileName& _mask)
{
#ifdef WINDOWS
	group = FindFirstFile(_path.GetAdd(_mask).c_str(), &ffd);
	valid = group != INVALID_HANDLE_VALUE;
#else//WINDOWS
	path = _path;
	mask = _mask;
	group = opendir(path.c_str());
	//LSD_DESIRE(group);
	Next();
#endif//WINDOWS
}
//=============================================================================
//	eSearch::End
//-----------------------------------------------------------------------------
void Search::End()
{
#ifdef WINDOWS
	FindClose(group);
#else//WINDOWS
	closedir(group);
#endif//WINDOWS
	group = NULL;
	Clear(ffd);
}
//=============================================================================
//	eSearch::Next
//-----------------------------------------------------------------------------
void Search::Next()
{
#ifdef WINDOWS
	valid = FindNextFile(group, &ffd) != 0;
#else//WINDOWS
	dirent* de = readdir(group);
	valid = (de != NULL) && (fnmatch(mask.c_str(), de->d_name, FNM_NOESCAPE) != FNM_NOMATCH);
	if(valid)
	{
		ffd = *de;
		FileName fn(path);
		fn.Path();
		fn.Add(Name());
		stat(fn.c_str(), &file_stat);
	}
#endif//WINDOWS
}
//=============================================================================
//	eSearch::IsFile
//-----------------------------------------------------------------------------
bool Search::IsFile() const
{
#ifdef WINDOWS
	return Valid() && !(ffd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY);
#else//WINDOWS
	return Valid() && ffd.d_type == DT_REG;
#endif//WINDOWS
}
//=============================================================================
//	eSearch::IsSubfolder
//-----------------------------------------------------------------------------
bool Search::IsSubfolder() const
{
#ifdef WINDOWS
	if(Valid() && (ffd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
	{
		return strcmp(ffd.cFileName, ".") && strcmp(ffd.cFileName, "..");
	}
#else//WINDOWS
	if(Valid() && ffd.d_type == DT_DIR)
	{
		return strcmp(ffd.d_name, ".") && strcmp(ffd.d_name, "..");
	}
#endif//WINDOWS
	return false;
}
//=============================================================================
//	eSearch::Name
//-----------------------------------------------------------------------------
const char* Search::Name() const
{
#ifdef WINDOWS
	return ffd.cFileName;
#else//WINDOWS
	return ffd.d_name;
#endif//WINDOWS
}

}//namespace xIO
