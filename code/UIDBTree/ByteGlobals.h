#pragma once

#include "Globals.h"

#define Byte unsigned char
#define ByteVector std::vector<Byte>
#define ByteVectorVector std::vector<ByteVector>

//======================================================================================================================

//To ByteVector conversions.

static inline ByteVector StringToBV(std::string str)
{
    return ByteVector(str.begin(), str.end());
}

static inline ByteVector WStringToBV(std::wstring wstr)
{
    return ByteVector(wstr.begin(), wstr.end());
}

template <typename T>
static inline ByteVector TToBV(T t)
{
    return ByteVector(reinterpret_cast<Byte*>(&t), reinterpret_cast<Byte*>(&t) + sizeof(T));
}

//======================================================================================================================

//To string conversions.

static inline std::string BVToString(ByteVector bv)
{
    return std::string(bv.begin(), bv.end());
}

static inline std::wstring BVToWString(ByteVector bv)
{
    return std::wstring(bv.begin(), bv.end());
}

static inline std::string BVVToString(ByteVectorVector bvv)
{
    std::stringstream ss;
    for (ByteVector bv: bvv)
    {
        ss << BVToString(bv);
    }
    return ss.str();
}

static inline std::wstring BVVToWString(ByteVectorVector bvv)
{
    std::wstringstream wss;
    for (ByteVector bv: bvv)
    {
        wss << BVToWString(bv);
    }
    return wss.str();
}

template <typename T>
static inline T BVToT(ByteVector bv)
{
    return *reinterpret_cast<T*>(bv.data());
}