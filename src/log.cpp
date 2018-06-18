#include "stdafx.h"
#include "log.h"
#include "file_name.h"
#include "system.h"
#include <fstream>
#include <cstdlib>

namespace xIO
{

const std::string logFileName = "uTestLog.txt";

std::vector<std::string>	Log::logs = {};
std::mutex					Log::muteVector = {};

void Log::Debug(std::string _log)
{
	muteVector.lock();
	logs.push_back(xSystem::GetCurrentTime() + _log);
	muteVector.unlock();
}

void Log::Error(std::string _log)
{
	muteVector.lock();
	logs.push_back(xSystem::GetCurrentTime() + "[ERROR] " + _log);
	muteVector.unlock();
}

void Log::Warning(std::string _log)
{
	muteVector.lock();
	logs.push_back(xSystem::GetCurrentTime() + "[WARNING] " + _log);
	muteVector.unlock();
}

void Log::Flush(std::string _path)
{
	FileName path(_path);
	if(!xSystem::Access(path.str(), xSystem::A_EXIST))
	{
		xSystem::CreateDir(path.str());
	}

	std::ofstream file(path.Path().GetAdd(logFileName).str(), std::ios_base::app);
	for(int32_t i = 0; i < logs.size(); i++)
	{
		file << logs[i];
		file << std::endl;
	}
	file.close();
}

const std::vector<std::string>& Log::Flush()
{
	return logs;
}

void Log::Clear()
{
	muteVector.lock();
	logs.clear();
	muteVector.unlock();
}

}//namespace xIO