#include "stdafx.h"
#include "system.h"
#include <stdio.h>

#ifdef WINDOWS
#	include <io.h>  
#	include <stdlib.h> 
#	include <direct.h>
#	define GetCurrentDir _getcwd
#else
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <unistd.h>
#	define GetCurrentDir getcwd
#endif

namespace xSystem
{

	bool Access(const std::string& _name, AccessMode _mode)
	{
#ifdef WINDOWS
		int32_t a = _access_s(_name.c_str(), _mode);
		return a == 0;
#else
		return access(_name.c_str(), _mode) == 0;
#endif
	}

	std::string GetCurrentWorkingDir()
	{
		char buff[FILENAME_MAX];
		GetCurrentDir(buff, FILENAME_MAX);
		std::string current_working_dir(buff);
		return current_working_dir;
	}


	void CreateDir(const std::string& _path)
	{
		int error = 0;
		#ifdef WINDOWS
		error = _mkdir(_path.c_str());
		#else 
		mode_t nMode = 0733;					// UNIX style permissions
		error = mkdir(_path.c_str(), nMode);	// can be used on non-Windows
		#endif
	}

}//namespace xSystem
