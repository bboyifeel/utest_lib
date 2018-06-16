#include "stdafx.h"
#include "test_block.h"

namespace xTest
{

//**************************************************************************************************
//	eTestBlock::Start
//--------------------------------------------------------------------------------------------------
bool eTestBlock::Start()
{
	//LSD_DESIRE(state == S_OPENED);
	State(S_STARTING);
	return true;
}
//==================================================================================================
//	eTestBlock::Load
//--------------------------------------------------------------------------------------------------
bool eTestBlock::Load(json& _jFile)
{
	if(!LoadIdent(_jFile))
	{
		//LSD_WARNING("[eTestBlock] Load fail");
		return false;
	}
	return true;
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
	return false;
}

}//namespace xTest