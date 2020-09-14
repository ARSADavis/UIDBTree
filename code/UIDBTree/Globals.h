#pragma once

#include <cstddef>
#include <memory>
#include <vector>
#include <map>
#include <optional>
#include <string>
#include <sstream>

#define ByteVector std::vector<unsigned char>
#define BVToString(bv) std::string(bv.begin(), bv.end())
#define BVToWString(bv) std::wstring(bv.begin(), bv.end())

static inline ByteVector StringToBV(std::string str)
{
    return ByteVector(str.begin(), str.end());
}