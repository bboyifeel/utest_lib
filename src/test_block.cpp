#include "stdafx.h"
#include "test_block.h"

namespace xTest
{

//**************************************************************************************************
//	eTestBlock::Start
//--------------------------------------------------------------------------------------------------
bool eTestBlock::Start()
{
	State(S_STARTING);
	return true;
}
//==================================================================================================
//	eTestBlock::Load
//--------------------------------------------------------------------------------------------------
bool eTestBlock::Load(json& _jFile)
{
	return LoadIdent(_jFile);
}
//==================================================================================================
//	eTestBlock::ConfigFileName
//--------------------------------------------------------------------------------------------------
void eTestBlock::ConfigFileName(const std::string& _fName)
{
	configFileName = _fName;
}
//==================================================================================================
//	eTestBlock::LoadIdent
//--------------------------------------------------------------------------------------------------
bool eTestBlock::LoadIdent(json& _jFile)
{
	bool ret = false;
	if(!_jFile["result"].is_null())
	{
		resultLoader = ResultLoader();
		ret = resultLoader->LoadResult(_jFile, this);
		xBase::SAFE_DELETE(resultLoader);
	}
	else
	{
		std::string message = "[eTestBlock] ";
					message += "\"";
					message += configFileName;
					message += "\"";
					message += " LoadIdent fail: Couldn't find ident - result";
		xIO::Log::Error(message);
	}
	return ret;
}
//==================================================================================================
//	eTestBlock::OnResultActual
//--------------------------------------------------------------------------------------------------
void eTestBlock::OnResultActual()
{
	State(S_COMPLETED);
}
//==================================================================================================
//	eTestBlock::State
//--------------------------------------------------------------------------------------------------
void eTestBlock::State(int32_t _state)
{
	if(state != _state)
	{
		int32_t oldState = state;
		state = _state;
		NextState(oldState);
	}
}

//**************************************************************************************************
//	eTestBlock::eResultLoader::LoadResult
//--------------------------------------------------------------------------------------------------
bool eTestBlock::eResultLoader::LoadResult(json& _jFile, eTestBlock* _testBlock)
{
	if(_jFile["result"].is_number_integer())
	{
		_testBlock->ResultExpected(_jFile["result"]);
		return true;
	}
	std::string message = "[eResultLoader] ";
				message += "\"";
				message += _testBlock->configFileName;
				message += "\"";
				message += " LoadResult fail";
	xIO::Log::Error(message);
	return false;
}

}//namespace xTest