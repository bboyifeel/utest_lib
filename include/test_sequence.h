#pragma once

#include "container_template.h"

namespace xTest
{

class TEST_API eTestBlock;
class TEST_API eTestBlocks_t;
class TEST_API eTestRunner;

//**************************************************************************************************
//	eTestSequence
//--------------------------------------------------------------------------------------------------
class TEST_API eTestSequence
{
public:
	eTestSequence();
	virtual ~eTestSequence();

	virtual bool		Start(const eTestRunner& _runner);
	virtual bool		Update();

	virtual bool		IsCompleted()		const;
	virtual bool		Load(json&);
protected:
	bool				UpdateCurrent();
	eTestBlock*			Current() const;
	void				CurrentStart();

	virtual bool		LoadItems(json&);
	virtual bool		LoadBlocks(json&);

	eTestBlocks_t*		blocks = nullptr;
	size_t				current = 0;
};

//**************************************************************************************************
//	eTestSequences
//--------------------------------------------------------------------------------------------------
class TEST_API eTestSequences : public eTestSequence, public eVectorGeneric<eTestSequence>
{
	using eInherited = eVectorGeneric<eTestSequence>;
public:
	virtual ~eTestSequences() = default;

	virtual bool	Start(const eTestRunner& _runner)		override;
	virtual bool	Update()								override;

	virtual bool	IsCompleted() const						override;
	virtual	bool	Load(json&)								override;
	virtual bool	LoadItems(json& _jFile, eTestSequence*&);
};

}//namespace xTest