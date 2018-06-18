#pragma once

#if defined(WINDOWS)
#	ifdef utest_lib_EXPORTS
#		define TEST_API __declspec(dllexport)
#	else//TEST_EXPORTS
#		define TEST_API __declspec(dllimport)
#	endif//TEST_EXPORTS
#else//WINDOWS
#	define TEST_API
#endif//WINDOWS

#include <map>
#include <vector>
#include <string>
#include <iostream>

#ifdef WINDOWS
#	include <windows.h>
#endif//WINDOWS

#include "ctpl.h"
#include "json.hpp"
using json = nlohmann::json;