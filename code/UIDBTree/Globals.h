#pragma once

#include <cstddef>
#include <memory>
#include <vector>
#include <map>
#include <optional>
#include <string>
#include <sstream>

#define ByteVector std::vector<unsigned char>
#define ByteVectorVector std::vector<ByteVector>

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

static inline std::string BVVToString(ByteVectorVector bvv)
{
    std::stringstream ss;
    for (ByteVector bv: bvv)
    {
        ss << BVToString(bv);
    }
    return "{ " + ss.str() + " }";
}

static inline std::wstring BVVToWString(ByteVectorVector bvv)
{
    std::wstringstream wss;
    for (ByteVector bv: bvv)
    {
        wss << BVToWString(bv);
    }
    return L"{ " + wss.str() + L" }";
}