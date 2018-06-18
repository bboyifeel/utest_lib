#include "stdafx.h"
#include "tester.h"
#include "system.h"
#include "file_search.h"
#include "test_runner.h"
#include "test_registration.h"
#include "ctpl.h"
#include <fstream>

namespace xTest
{

const int32_t defaultTheadCount = 1;

//**************************************************************************************************
//	eTester::eTester
//--------------------------------------------------------------------------------------------------
eTester::eTester()
{
	logsPath = xIO::FileName(xSystem::GetCurrentWorkingDir());
	logsPath.Path().Add("Logs");
}
//==================================================================================================
//	eTester::ThreadPool
//--------------------------------------------------------------------------------------------------
eTester::~eTester()
{
	Done();
}
//==================================================================================================
//	eTester::ThreadPool
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

int32_t eTester::ThreadCount()
{
	static unsigned concurentThreadsSupported = std::thread::hardware_concurrency() ?
													std::thread::hardware_concurrency() : defaultTheadCount;
	return concurentThreadsSupported;
}

//==================================================================================================
//	eTester::ThreadPool
//--------------------------------------------------------------------------------------------------
ctpl::thread_pool& eTester::ThreadPool()
{

	static bool		initialization = true;
	static ctpl::thread_pool pool;

	if(initialization)
	{
		initialization = false;
		
		pool.resize(this->ThreadCount());
		
		std::string message = "[eTester] ThreadPool: Created ";
					message += std::to_string(pool.size());
					message += " threads";
		xIO::Log::Debug(message);
	}

	return pool;
}

//==================================================================================================
//	eTester::Init
//--------------------------------------------------------------------------------------------------
bool eTester::Init()
{
	this->ThreadPool(); //Initialization
	configsPath.Path();

	if(!xSystem::Access(configsPath.str(), xSystem::A_READ))
	{
		std::string	message = "[eTester] Fail to access path ";
					message += "\"";
					message += configsPath.str();
					message += "\"";
		xIO::Log::Error(message);
		return false;
	}
	std::string	message = "[eTester] Path successfully accessed ";
				message += "\"";
				message += configsPath.str();
				message += "\"";
	xIO::Log::Debug(message);
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
	xIO::Log::Flush(logsPath.str());
	xIO::Log::Clear();
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
		results[cfgSearch.Name()] = false;
	}
	std::string message = "[eTester] Run: Found ";
				message += std::to_string(results.size());
				message += " JSON ";
				message += results.size() != 1 ?	"files" : "file";
				message += " in folder ";
				message += "\"";
				message += configsPath.str();
				message += "\"";
	xIO::Log::Debug(message);

	std::vector<std::future<void>> resultsList;

	for(eResults::iterator it = results.begin(); it != results.end(); ++it)
	{
		std::string fileName = it->first;
		std::future<void> qt = this->ThreadPool().push(StartJSON, std::ref(*this), std::move(fileName));
		resultsList.push_back(std::move(qt));
	}
	
	for(auto &item : resultsList)
	{
		item.wait();
	}
	for(auto &item : results)
	{
		succeeded &= item.second;
	}

	xIO::Log::Debug("\n" + ToString(results));
}
//==================================================================================================
//	eTester::DoRunner
//--------------------------------------------------------------------------------------------------
void eTester::DoRunner(std::string _configName)
{
	std::string		fullPathWName = configsPath.GetAdd(_configName).str();
	std::ifstream	file(fullPathWName);
	json	jFile;
	jFile = json::parse(file);	
	file.close();

	std::string message = "[eTester] ";
				message += "\"";
				message += _configName;
				message += "\"";

	xIO::Log::Debug(message + " File loading started");

	if(jFile["test"].is_null())
	{
		message += " DoRunner fail: invalid signature, it must start with \"test\"";
		xIO::Log::Error(message);

		results[_configName] = false;
		return;
	}
	
	eTestRunner*			runner			= nullptr;
	if(!jFile["test"]["runner_name"].is_null())
	{
		std::string runnerClassName = jFile["test"]["runner_name"];
		runner = CreateRunner(runnerClassName);
	}
	else
	{
		runner = new eTestRunner;
	}

	runner->ConfigFileName(_configName);
	bool result = runner->Load(jFile["test"]);
	
	if(result)
	{
		message += " DoRunner: File successfully loaded";
		xIO::Log::Debug(message);
		result = runner->Do();
	}
	else
	{
		message += " DoRunner: Fail to load file";
		xIO::Log::Error(message);
	}

	xBase::SAFE_DELETE(runner);
	results[_configName] = result;
}

//**************************************************************************************************
//	ToString
//--------------------------------------------------------------------------------------------------
void StartJSON(int id, eTester& tester, std::string fileName)
{
	tester.DoRunner(std::move(fileName));
}

//**************************************************************************************************
//	ToString
//--------------------------------------------------------------------------------------------------
std::string ToString(const eTester::eResults& _results)
{
	std::string res;
 	for(auto const &item : _results)
 	{
		res += item.second ? "[Passed]" : "[Failed]";
		res += " - ";
		res += item.first;
		res += "\n";
	}
	return res;
}


}//namespace xTest