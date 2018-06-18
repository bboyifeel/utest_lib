#pragma once

#include <vector>
#include <string>

namespace xIO
{

class Log
{
public:
	static void								Debug(std::string log);
	static void								Error(std::string log);
	static void								Warning(std::string log);
	static void								Flush(std::string path);
	static const std::vector<std::string>&	Flush();
	static void								Clear();
protected:
	static std::vector<std::string> logs;
	static std::mutex				muteVector;
};

}//namespace xIO
