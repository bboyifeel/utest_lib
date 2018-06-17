#include "stdafx.h"
#include "test_runner.h"
#include "test_sequence.h"
#include "test_block.h"
#include "test_registration.h"

namespace xTest
{

const int32_t dispatchPeriod = 5;

//**************************************************************************************************
//	eTestRunner::Do
//--------------------------------------------------------------------------------------------------
bool eTestRunner::Do()
{
	std::string message = "[eTestRunner] ";
				message += "\"";
				message += configFileName;
				message += "\"";
				message += " Do: Test execution started";
	xIO::Log::Debug(message);

	bool result = isLoaded
		&& Init()
		&& sequences->Start(*this);
	if(result)
	{
		while(!sequences->IsCompleted())
		{
			if(!sequences->Update())
			{
				result = false;
				break;			// failure
			}
			::Sleep(dispatchPeriod);
		}
	}
	Done();
	return result;
}
//==================================================================================================
//	eTestRunner::Init
//--------------------------------------------------------------------------------------------------
bool eTestRunner::Init()
{
	if(initBlock && !initBlock->Start())
	{
		xIO::Log::Error("[eTestRunner] Init fail: initializer is not started");
		return false;
	}
	while(initBlock && !initBlock->IsCompleted())
	{
		::Sleep(dispatchPeriod);
	}
	if(initBlock && initBlock->ResultActual() != initBlock->ResultExpected())
	{
		std::string message = "[eTestRunner] Init fail: Actual/";
					message += initBlock->ResultActual();
					message += ", expect/";
					message += initBlock->ResultExpected();
		xIO::Log::Error(message);
		return false;
	}
	return true;
}
//==================================================================================================
//	eTestRunner::Done
//--------------------------------------------------------------------------------------------------
void eTestRunner::Done()
{
	sequences->Clear();
	xBase::SAFE_DELETE(sequences);
	
	if(initBlock)
		initBlock->Done();

	xBase::SAFE_DELETE(initBlock);
}
//==================================================================================================
//	eTestRunner::CreateSequences
//--------------------------------------------------------------------------------------------------
eTestSequences* eTestRunner::CreateSequences()
{
	return new eTestSequences;
}
//==================================================================================================
//	eTestRunner::Load
//--------------------------------------------------------------------------------------------------
bool eTestRunner::Load(json& _jFile)
{
	sequences = CreateSequences();
	sequences->ConfigFileName(configFileName);

	if(!LoadIdent(_jFile))
	{
		return false;
	}

	isLoaded = !sequences->empty();
	return isLoaded;
}
//==================================================================================================
//	eTestRunner::ConfigFileName
//--------------------------------------------------------------------------------------------------
void eTestRunner::ConfigFileName(const std::string& _fName)
{
	configFileName = _fName;
}
//==================================================================================================
//	eTestRunner::LoadIdent
//--------------------------------------------------------------------------------------------------
bool eTestRunner::LoadIdent(json& _jFile)
{
	std::string message = "[eTestRunner] ";
				message += "\"";
				message += configFileName;
				message += "\"";
				message += " LoadIdent fail: ";
	if(_jFile["name"].is_null())
	{
		message += " Couldn't find ident - name";
		xIO::Log::Error(message);
		return false;
	}

	std::string tmpName = _jFile["name"];
	name = tmpName; //must be unique for each test

	if(_jFile["sequences"].is_null())
	{
		message += "Couldn't find ident - sequences";
		xIO::Log::Error(message);
		return false;
	}

	if(!sequences->Load(_jFile["sequences"]))
	{
		return false;
	}
	
	if(!_jFile["initializer"].is_null())
	{
		std::string name = _jFile["initializer"]["name"];
		initBlock = CreateBlock(name);
		if(!initBlock)
		{
			std::string message = "[eTestRunner] ";
						message += "\"";
						message += configFileName;
						message += "\"";
						message += " LoadIdent:";
						message += " Unknown initializer name: ";
						message += name;
			xIO::Log::Warning(message);
			return false;
		}
		if(!initBlock->Load(_jFile["initializer"]))
		{
			return false;
		}
		
	}
	else
	{
		message += "Couldn't find ident - initializer";
		xIO::Log::Warning(message);
	}

	return true;
}

}//namespace xTest