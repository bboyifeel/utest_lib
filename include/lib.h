#pragma once

#if defined(WINDOWS)
#	ifdef test_lib_EXPORTS
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

#include "json.hpp"
using json = nlohmann::json;