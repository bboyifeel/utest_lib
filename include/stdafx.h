#pragma once

#include "lib.h"
#include "utils.h"
#include <string>
#include <iostream>
#include <cstdint>

#ifdef WINDOWS
#	include <windows.h>
#endif//WINDOWS

#include "json.hpp"
using json = nlohmann::json;