#pragma once

#include "file_name.h"

namespace xTest
{

class eTestRunner;

//**************************************************************************************************
//	eTester
//--------------------------------------------------------------------------------------------------
class TEST_API eTester
{
public:
	using eResults = std::map<std::string, bool>;

	~eTester()										{ Done();			}

	bool					Do(const std::string& path);
	const eResults&			Results()		const	{ return results;	}
	bool					IsSucceeded()			{ return succeeded;	}

protected:
	bool					Init();
	void					Done();
	void					Run();
	bool					DoRunner(const std::string& config);

	bool					isInitialized	= false;
	bool					succeeded		= false;
	eTestRunner*			runner			= nullptr;
	eResults				results;
	xIO::FileName			configsPath;
	json					jFile;
};

std::string TEST_API ToString(const eTester::eResults&);

}//namespace xTest