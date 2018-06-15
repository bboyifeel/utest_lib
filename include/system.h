#pragma once

#ifndef WINDOWS
#include <unistd.h>
#endif

namespace xSystem
{

#ifdef WINDOWS
enum AccessMode { A_EXIST = 0, A_WRITE = 2, A_READ = 4, A_READWRITE = 6 };
#else//WINDOWS
enum AccessMode { A_EXIST = F_OK, A_WRITE = W_OK, A_READ = R_OK, A_READWRITE = A_WRITE | A_READ };
#endif//WINDOWS
bool				Access(const std::string& name, AccessMode mode = A_EXIST);

}//xSystem
