#pragma once

#include "Globals.h"

enum class UIDBTreeResultCode: unsigned char
{
    Success,
    Error,
    Size
};

static std::map<UIDBTreeResultCode, std::string> UIDBTreeResultCodeMap({
    { UIDBTreeResultCode::Success, "Success" },
    { UIDBTreeResultCode::Error, "Error" },
    { UIDBTreeResultCode::Size, "Size" },
});