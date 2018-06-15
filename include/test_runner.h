#pragma once

namespace xTest
{
	
class TEST_API eTestBlock;
class TEST_API eTestSequences;

//**************************************************************************************************
//	eTestRunner
//--------------------------------------------------------------------------------------------------
class TEST_API eTestRunner
{
public:
	virtual ~eTestRunner() = default;

	bool						Do();

	bool						Load(json&);
protected:
	virtual bool				Init();
	virtual void				Done();
	virtual eTestSequences*		CreateSequences();
	virtual bool				LoadIdent(json&);

	std::string					name;
	bool						isLoaded	= false;
	eTestBlock*					initBlock	= nullptr;
	eTestSequences*				sequences	= nullptr;
};

}//namespace xTest