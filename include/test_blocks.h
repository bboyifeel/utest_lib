#pragma once

#include "json.hpp"
#include "test_block.h"
#include "test_registration.h"
#include "json.hpp"
using json = nlohmann::json;

namespace xTest
{

//**************************************************************************************************
//	eTestBlocksBase
//--------------------------------------------------------------------------------------------------
template <class T> class eTestBlocksBase : public T
{
	using eInherited = T;
public:
	virtual ~eTestBlocksBase()		{ items.Clear(); }

	virtual void		Done()									override;
	virtual bool		Start()									override;

	virtual bool		IsCompleted() const						override;
	virtual bool		Load(json&)								override;
	static eTestBlock*	LoadBlock(json&);

protected:
	eTestBlocks_t		items;
};
using eTestBlocks = eTestBlocksBase<eTestBlock>;

//**************************************************************************************************
//	eTestBlocksBase::Done
//--------------------------------------------------------------------------------------------------
template <class T> void eTestBlocksBase<T>::Done()
{
	for(eTestBlock* block : items)
	{
		block->Done();
	}
}
//==================================================================================================
//	eTestBlocksBase::Start
//--------------------------------------------------------------------------------------------------
template <class T> bool eTestBlocksBase<T>::Start()
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
template <class T> bool eTestBlocksBase<T>::IsCompleted() const
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
//	eTestBlocksBase::Load
//--------------------------------------------------------------------------------------------------
template <class T> bool eTestBlocksBase<T>::Load(json& _jFile)
{
	for(int i = 0; i < _jFile.size(); i++)
	{
		if(!_jFile[i]["block"].is_null())
		{
			eTestBlock* item = eTestBlocksBase::LoadBlock(_jFile[i]["block"]);
			if(!item)
			{
				return false;
			}
			items.Add(item);
		}
		else
		{
			xIO::Log::Warning("[eTestBlocks] Load fail");
			return false;
		}
	}

	return true;
}
//==================================================================================================
//	eTestBlocksBase::LoadBlock
//--------------------------------------------------------------------------------------------------
template <class T> eTestBlock* eTestBlocksBase<T>::LoadBlock(json& _jFile)
{
	if (_jFile["name"].is_null())
		return nullptr;

	std::string name = _jFile["name"];
	eTestBlock* block = CreateBlock(name);
	if(!block)
	{
		xIO::Log::Warning("[eTestBlocks] Load fail: unknown test block name");
		return nullptr;
	}
	if(!block->Load(_jFile))
	{
		xBase::SAFE_DELETE(block);
		return nullptr;
	}
	return block;
}

}//namespace xTest
