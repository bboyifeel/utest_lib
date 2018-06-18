#pragma once

#include "file_name.h"

namespace ctpl
{
	class thread_pool;
}

namespace xTest
{

class eTestRunner;

//**************************************************************************************************
//	eTester
//--------------------------------------------------------------------------------------------------
class TEST_API eTester
{
	friend void				StartJSON(int id, eTester& tester, std::string fileName);
public:
	using eResults = std::map<std::string, bool>;
	eTester();
	~eTester();

	bool					Do(const std::string& path);
	const eResults&			Results()		const	{ return results;	}
	bool					IsSucceeded()			{ return succeeded;	}
	int32_t					ThreadCount();
	ctpl::thread_pool&		ThreadPool();
protected:
	bool					Init();
	void					Done();
	void					Run();
	void					DoRunner(std::string config);

	bool					isInitialized	= false;
	bool					succeeded		= false;
	eResults				results;
	xIO::FileName			configsPath;
	xIO::FileName			logsPath;
};

std::string TEST_API ToString(const eTester::eResults&);

}//namespace xTest