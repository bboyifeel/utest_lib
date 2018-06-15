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