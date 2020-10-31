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

#include "ByteGlobals.h"

#define TreeKeyType size_t

static std::wstring StringToWString(std::string convertMe)
{
    return std::wstring(convertMe.begin(), convertMe.end());
}

static std::string TimeToString(time_t* convertMe)
{
    std::string almostThere = ctime(convertMe);
    return almostThere.substr(0, almostThere.length() - 1);
}

static std::wstring TimeToWString(time_t* convertMe)
{
    return StringToWString(TimeToString(convertMe));
}