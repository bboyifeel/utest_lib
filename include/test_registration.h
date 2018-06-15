#pragma once

#include "entry.h"
#include "utils.h"

namespace xTest
{

class eTestBlock;
class eTestRunner;

//**************************************************************************************************
//	eBlockRegisterBase
//--------------------------------------------------------------------------------------------------
template<class T> class TEST_API eBlockRegisterBase
{
public:
	virtual ~eBlockRegisterBase()
	{}

	typedef eTestBlock* (eCreateFunction)();
	typedef Entry<eCreateFunction*, T> eRegistration;
};
using eBlockRegister = eBlockRegisterBase<std::string>;

//**************************************************************************************************
//	CreateBlock
//--------------------------------------------------------------------------------------------------
TEST_API	eTestBlock*			CreateBlock(const std::string& name);

//**************************************************************************************************
//	CreateBlock
//--------------------------------------------------------------------------------------------------
template <typename T> T* 	CreateBlock()
{
	return CAST(T, CreateBlock((std::string)T::NAME));
}

//**************************************************************************************************
//	TEST_API
//--------------------------------------------------------------------------------------------------
template <class T> 
class TEST_API eRunnerRegisterBase
{
public:
	virtual ~eRunnerRegisterBase() = default;

	typedef eTestRunner* (eCreateFunction)();
	typedef Entry<eCreateFunction*, T> eRegistration;
};
using eRunnerRegister = eRunnerRegisterBase<std::string>;

TEST_API	eTestRunner*			CreateRunner(const std::string& name);

}//namespace xTest

template class TEST_API xTest::eBlockRegisterBase<std::string>::eRegistration::eDef;
template class TEST_API xTest::eRunnerRegisterBase<std::string>::eRegistration::eDef;

#define TEST_BLOCK_REGISTER(cls)																\
static xTest::eTestBlock*						_Create_##cls() { return new cls(); }			\
static xTest::eBlockRegister::eRegistration		Registration_Block_##cls(cls::NAME, _Create_##cls)

#define TEST_BLOCK_DECLARE_BEGIN(cls, clsbase, name)											\
class cls : public clsbase																		\
{																								\
	using eInherited = clsbase;																	\
public:																							\
	static constexpr const char* NAME = name;													\
																								\
	cls() : eInherited(NAME) {}																	\
	virtual ~cls() = default;																	\
protected:																						\
	virtual bool		LoadIdent(json&	)			override;

#define TEST_BLOCK_DECLARE_END																	\
};

#define TEST_RUNNER_REGISTER(cls)																\
static xTest::eTestRunner*							_Create_##cls() { return new cls(); }		\
static xTest::eRunnerRegister::eRegistration		Registration_Runner_##cls(cls::NAME, _Create_##cls)

