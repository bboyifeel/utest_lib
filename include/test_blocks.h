#pragma once

#include "test_block.h"
#include "test_registration.h"

namespace xTest
{

//**************************************************************************************************
//	eTestBlocks
//--------------------------------------------------------------------------------------------------
class eTestBlocks : public eTestBlock
{
	using eInherited = eTestBlock;
public:
	virtual ~eTestBlocks()		{ items.Clear(); }

	virtual void		Done()									override;
	virtual bool		Start()									override;

	virtual bool		IsCompleted() const						override;
	virtual bool		Load(json&)								override;
	static eTestBlock*	LoadBlock(json&, const std::string& configFileName);

protected:
	eTestBlocks_t		items;
};

}//namespace xTest
