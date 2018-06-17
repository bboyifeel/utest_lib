#include "stdafx.h"
#include "test_blocks.h"

namespace xTest
{

//**************************************************************************************************
//	eTestBlocks::Done
//--------------------------------------------------------------------------------------------------
void eTestBlocks::Done()
{
	for(eTestBlock* block : items)
	{
		block->Done();
	}
}
//==================================================================================================
//	eTestBlocks::Start
//--------------------------------------------------------------------------------------------------
bool eTestBlocks::Start()
{
	for(eTestBlock* block : items)
	{
		if(!block->Start())
		{
			return false;
		}
	}
	return true;
}
//==================================================================================================
//	eTestBlocks::IsCompleted
//--------------------------------------------------------------------------------------------------
bool eTestBlocks::IsCompleted() const
{
	for(eTestBlock* block : items)
	{
		if(!block->IsCompleted())
		{
			return false;
		}
	}
	return true;
}
//==================================================================================================
//	eTestBlocks::Load
//--------------------------------------------------------------------------------------------------
bool eTestBlocks::Load(json& _jFile)
{
	for(int i = 0; i < _jFile.size(); i++)
	{
		if(!_jFile[i]["block"].is_null())
		{
			eTestBlock* item = eTestBlocks::LoadBlock(_jFile[i]["block"], configFileName);
			if(!item)
			{
				return false;
			}
			items.Add(item);
		}
		else
		{
			return false;
		}
	}

	return true;
}
//==================================================================================================
//	eTestBlocks::LoadBlock
//--------------------------------------------------------------------------------------------------
eTestBlock* eTestBlocks::LoadBlock(json& _jFile, const std::string& _configFileName)
{
	std::string message = "[eTestBlocks] ";
				message += "\"";
				message += _configFileName;
				message += "\"";
				message += " LoadBlock fail: ";
	
	if(_jFile["name"].is_null())
	{
		message += "Couldn't find ident - name";
		xIO::Log::Error(message);
		return nullptr;
	}

	std::string name = _jFile["name"];
	eTestBlock* block = CreateBlock(name);
	if(!block)
	{
		message += "Unknown test block name";
		xIO::Log::Error(message);
		return nullptr;
	}
	if(!block->Load(_jFile))
	{
		xBase::SAFE_DELETE(block);
		return nullptr;
	}
	return block;
}

}