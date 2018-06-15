#include "stdafx.h"
#include "system.h"

#ifdef WINDOWS
#	include <io.h>  
#	include <stdio.h>  
#	include <stdlib.h> 
#else
#	include <unistd.h>
#endif

namespace xSystem
{

	bool Access(const std::string& _name, AccessMode _mode)
	{
#ifdef WINDOWS
		int32_t a = _access_s(_name.c_str(), _mode);
		return a == 0;
#else WINDOWS
		return access(_name.c_str(), _mode) == 0;
#endif WINDOWS
	}

}//namespace xSystem