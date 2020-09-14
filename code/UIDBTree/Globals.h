#pragma once

#include <cstddef>
#include <memory>
#include <vector>
#include <map>
#include <optional>
#include <string>
#include <sstream>

#define ByteVector std::vector<unsigned char>

static inline std::string BVToString(ByteVector bv)
{
    return std::string(bv.begin(), bv.end());
}

static inline std::wstring BVToWString(ByteVector bv)
{
    return std::wstring(bv.begin(), bv.end());
}

static inline ByteVector StringToBV(std::string str)
{
    return ByteVector(str.begin(), str.end());
}