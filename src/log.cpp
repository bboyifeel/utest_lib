#include "stdafx.h"
#include "log.h"
#include "file_name.h"
#include <fstream>

namespace xIO
{

const std::string logFileName = "uTestLog.txt";

std::vector<std::string> Log::logs = {};

void Log::Debug(std::string _log)
{
	logs.push_back(_log);
}

void Log::Error(std::string _log)
{
	logs.push_back("[ERROR] " + _log);
}

void Log::Warning(std::string _log)
{
	logs.push_back("[WARNING] " + _log);
}

void Log::Flush(std::string _path)
{
	FileName path(_path);
	std::ofstream file(path.Path().GetAdd(logFileName).str(), std::ios_base::ate);
	for(int32_t i = 0; i < logs.size(); )
	{
		file << std::endl;
		file << logs[i];
	}
	file.close();
}

const std::vector<std::string>& Log::Flush()
{
	return logs;
}

}//namespace xIO