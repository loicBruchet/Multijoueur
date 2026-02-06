#pragma once

#ifdef _DEBUG
#pragma comment(lib, "../../ide/SMLib/x64/debug/SMLib.lib")
#else
#pragma comment(lib, "../../ide/SMLib/x64/release/SMLib.lib")
#endif

#include "../SMLib/framework.h"
#include "App.h"