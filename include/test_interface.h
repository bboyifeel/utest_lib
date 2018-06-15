#pragma once

/*
#include <lsd_base/console_interface.h>
#include <lsd_log/log.h>
#include <lsd_log/log_macro.h>

_EXTERN_LOG(main)
#define MAIN_LOG(lvl, exp)				__LOG(main, lvl, exp)
#define MAIN_LOG_EX(exp, lvl, subname)	__LOG_EX(main, lvl, subname, exp)

namespace xTest
{

// *****************************************************************************
//	eOutputInterface
//-----------------------------------------------------------------------------
class eOutputInterface : public xConsole::eOutputInterfaceBase
{
	typedef xConsole::eOutputInterfaceBase eInherited;
public:
	virtual ~eOutputInterface() = default;

	virtual void	Write(const string& msg, int32_t lvl = LEVEL_CRITICAL, const string& subname = "");
};

}//xTest*/