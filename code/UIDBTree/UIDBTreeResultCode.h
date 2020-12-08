#pragma once

#include "../Globals.h"

enum class UIDBTreeResultCode: unsigned char
{
    Success,
    SuccessDuplicateValueAppended,

    NoNodeFound,

    Error,
    NoDuplicatesAllowedError,

    Size
};

static std::map<UIDBTreeResultCode, std::wstring> UIDBTreeResultCodeMap({
    { UIDBTreeResultCode::Success, L"Success" },
    { UIDBTreeResultCode::SuccessDuplicateValueAppended, L"Success: duplicate value appended." },

    { UIDBTreeResultCode::NoNodeFound, L"No node found." },

    { UIDBTreeResultCode::Error, L"Error!" },
    { UIDBTreeResultCode::NoDuplicatesAllowedError, L"Error: no duplicates allowed!" },

    { UIDBTreeResultCode::Size, L"INVALID RESULT CODE" }
});