#pragma once

#include <vector>
#include <fstream>
#include <iostream>
#include <initializer_list>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <filesystem>
#include <array>
#include <memory>

#ifdef TMT_PLATFORM_WINDOWS
	#include <Windows.h>
	#undef max;
	#undef min;
#endif // TMT_PLATFORM_WINDOWS
