#pragma once

#include "file_name.h"
#ifndef WINDOWS
#	include <dirent.h>
#	include <sys/stat.h>
#endif//WINDOWS

namespace xIO
{

//=============================================================================
//	eSearch
//-----------------------------------------------------------------------------
class Search
{
public:
	Search(const FileName& path, const FileName& mask = "*.*") { Begin(path, mask); }
	~Search()	{ End(); }

	void			Begin(const FileName& path, const FileName& mask = "*.*");
	bool			Valid() 		const { return valid; }
	void			Next();
	void			End();

	bool			IsFile() 		const;
	bool			IsSubfolder()	const;
	const char*		Name() 			const;
#ifdef WINDOWS
#ifdef X64
	size_t			Size() 			const { return (size_t)((((size_t)ffd.nFileSizeHigh) << 32) + ffd.nFileSizeLow); }
#else
	size_t			Size() 			const { return (size_t)ffd.nFileSizeLow; }
#endif//X64
#else//WINDOWS
	size_t			Size()			const { return (size_t)file_stat.st_size; }
#endif//WINDOWS

protected:
#ifdef WINDOWS
	HANDLE			group;
	WIN32_FIND_DATA	ffd;
#else//WINDOWS
	FileName		path;
	FileName		mask;
	DIR*			group;
	dirent			ffd;
	struct stat		file_stat;
#endif//WINDOWS
	bool			valid;
};

}//namespace xIO
