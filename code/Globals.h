#pragma once

#include <cstddef>
#include <memory>
#include <vector>
#include <map>
#include <ranges>
#include <optional>
#include <string>
#include <sstream>
#include <fstream>
#include <thread>
#include <chrono>
#include <ctime>
#include <codecvt>
#include <exception>
#include <functional>

#include "ByteGlobals.h"

#define TreeKeyType size_t

static std::wstring StringToWString(std::string convertMe)
{
    return std::wstring(convertMe.begin(), convertMe.end());
}

static std::string TimeToString(time_t* convertMe)
{
    return std::to_string(*convertMe);
}

static std::wstring TimeToWString(time_t* convertMe)
{
    return std::to_wstring(*convertMe);
}