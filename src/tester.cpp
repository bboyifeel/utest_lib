#include "stdafx.h"
#include "tester.h"
#include "system.h"
#include "file_search.h"
#include "test_runner.h"
#include "test_registration.h"

#include <fstream>

namespace xTest
{

//**************************************************************************************************
//	eTester::Do
//--------------------------------------------------------------------------------------------------
bool eTester::Do(const std::string& _path)
{
	configsPath = _path;
	Done();
	if(Init())
	{
		Run();
	}
	return IsSucceeded();
}
//==================================================================================================
//	eTester::Init
//--------------------------------------------------------------------------------------------------
bool eTester::Init()
{
	configsPath.Path();
	if(!xSystem::Access(configsPath.str(), xSystem::A_READWRITE))
	{
		return false;
	}

	isInitialized = true;
	return true;
}
//==================================================================================================
//	eTester::Done
//--------------------------------------------------------------------------------------------------
void eTester::Done()
{
	if(isInitialized)
	{
		results.clear();
		succeeded = false;
		isInitialized = false;
	}
}
//==================================================================================================
//	eTester::Run
//--------------------------------------------------------------------------------------------------
void eTester::Run()
{
	xIO::Search cfgSearch(configsPath.str(), "*.json");
	succeeded = cfgSearch.Valid();

	for(; cfgSearch.Valid(); cfgSearch.Next())
	{
		std::string fullPathWName = configsPath.GetAdd(cfgSearch.Name()).str();
		results[cfgSearch.Name()] = DoRunner(fullPathWName);
		succeeded &= results[cfgSearch.Name()];
	}
}
//==================================================================================================
//	eTester::DoRunner
//--------------------------------------------------------------------------------------------------
bool eTester::DoRunner(const std::string& _config)
{
	std::ifstream file(_config);
	jFile = json::parse(file);
	file.close();

	if(jFile["test"].is_null())
	{
		//LSD_WARNING(format("[eTestRunner] Load fail: invalid signature, it must start with \"test\""));
		return false;
	}

	if(!jFile["test"]["runner_name"].is_null())
	{
		std::string runnerClassName = jFile["test"]["runner_name"];
		runner = CreateRunner(runnerClassName);
	}
	else
	{
		runner = new eTestRunner;
	}

	bool result = runner->Load(jFile["test"]);
	
	if(result)
	{
		result = runner->Do();
	}

	xBase::SAFE_DELETE(runner);
	return result;
}

//**************************************************************************************************
//	ToString
//--------------------------------------------------------------------------------------------------
std::string ToString(const eTester::eResults& _results)
{
	std::string res;
 	for(auto const &item : _results)
 	{
		res += item.first;
		res += "\t";
		res += "- ";
		res += item.second ? "Passed" : "Failed";
		res += "\n";
	}
	return res;
}

}//namespace xTest