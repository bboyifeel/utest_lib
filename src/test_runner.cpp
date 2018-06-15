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
		//LSD_WARNING("[eTestRunner] Init fail: initializer is not started");
		return false;
	}
	while(initBlock && !initBlock->IsCompleted())
	{
		::Sleep(dispatchPeriod);
	}
	if(initBlock && initBlock->ResultActual() != initBlock->ResultExpected())
	{
		/*LSD_WARNING(format("[eTestRunner] Init fail: initializer result is not expected. Actual/%1%, expect/%2%")
			% initBlock->ResultActual()
			% initBlock->ResultExpected());
*/
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
	SAFE_DELETE(sequences);
	SAFE_CALL(initBlock)->Done();
	SAFE_DELETE(initBlock);
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
	if(!LoadIdent(_jFile))
	{
		//LSD_WARNING("[eTestRunner] Load fail");
		return false;
	}

	isLoaded = !sequences->empty();
	return isLoaded;
}
//==================================================================================================
//	eTestRunner::LoadIdent
//--------------------------------------------------------------------------------------------------
bool eTestRunner::LoadIdent(json& _jFile)
{
	if(_jFile["name"].is_null())
	{
		return false;
	}

	std::string tmpName = _jFile["name"];
	name = tmpName; //must be unique for each test

	if(_jFile["sequences"].is_null() || !sequences->Load(_jFile["sequences"]))
	{
		return false;
	}
	
	if(!_jFile["initializer"].is_null())
	{
		std::string name = _jFile["initializer"]["name"];
		initBlock = CreateBlock(name);
		if(!initBlock)
		{
			//LSD_WARNING(format("[eTestRunner] Load fail: unknown initializer name: %1%") % name);
			return false;
		}
		if(!initBlock->Load(_jFile["initializer"]))
			return false;
	}

	return true;
}

}//namespace xTest
