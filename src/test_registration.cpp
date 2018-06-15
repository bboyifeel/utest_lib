#include "stdafx.h"
#include "test_registration.h"

namespace xTest
{

//**************************************************************************************************
//	CreateBlock
//--------------------------------------------------------------------------------------------------
TEST_API eTestBlock*  CreateBlock(const std::string& _name)
{
	eBlockRegister::eRegistration* reg = eBlockRegister::eRegistration::Get(_name);
	return reg ? (reg->Info())() : nullptr;
}

//**************************************************************************************************
//	CreateRunner
//--------------------------------------------------------------------------------------------------
TEST_API eTestRunner* CreateRunner(const std::string& _name)
{
	eRunnerRegister::eRegistration* reg = eRunnerRegister::eRegistration::Get(_name);
	return reg ? (reg->Info())() : nullptr;
}

}//namespace xTest
