#pragma once

#include "Globals.h"

enum class UIDBTreeResultCode: unsigned char
{
    Success,
    Error,
    Size
};

static std::map<UIDBTreeResultCode, std::wstring> UIDBTreeResultCodeMap({
    { UIDBTreeResultCode::Success, L"Success" },
    { UIDBTreeResultCode::Error, L"Error" },
    { UIDBTreeResultCode::Size, L"Size" }
});