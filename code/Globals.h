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
#include <cmath>

#include "ByteGlobals.h"

#define TreeKeyType intptr_t

static std::wstring StringToWString(std::string const& convertMe)
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

static std::wstring PadTheLeft(std::wstring const& padMe, wchar_t const& withMe, std::intptr_t desiredLength)
{
    std::intptr_t lengthDifference = desiredLength - padMe.length();
    if (lengthDifference > 0)
    {
        return std::wstring(lengthDifference, withMe) + padMe;
    }
    else
    {
        return padMe;
    }
}

static std::wstring PadTheRight(std::wstring const& padMe, wchar_t const& withMe, std::intptr_t desiredLength)
{
    std::intptr_t lengthDifference = desiredLength - padMe.length();
    if (lengthDifference > 0)
    {
        return padMe + std::wstring(lengthDifference, withMe);
    }
    else
    {
        return padMe;
    }
}

static std::wstring PadCentered(std::wstring const& padMe, wchar_t const& withMe, std::intptr_t desiredLength)
{
    std::intptr_t lengthDifference = desiredLength - padMe.length();
    if (lengthDifference > 0)
    {
        return std::wstring((size_t)ceil((float)lengthDifference / 2.0), withMe) + padMe +
            std::wstring((size_t)floor((float)lengthDifference / 2.0), withMe);
    }
    else
    {
        return padMe;
    }
}