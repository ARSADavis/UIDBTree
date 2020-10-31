#pragma once

#include "../Globals.h"

//├─┬─┐ ╘═╤═╕
#define UIDBTREESHORTPRINT
#ifdef UIDBTREESHORTPRINT
    static std::wstring UIDBTreeRightChild = L"╞";
    static std::wstring UIDBTreeLeftChild = L"└";
    static std::wstring UIDBTreeAncestorNext = L"│ ";
    static std::wstring UIDBTreeAncestorNoMore = L"  ";
#else
    static std::wstring UIDBTreeRightChild = L"╞═";
    static std::wstring UIDBTreeLeftChild = L"└─";
    static std::wstring UIDBTreeAncestorNext = L" │ ";
    static std::wstring UIDBTreeAncestorNoMore = L"   ";
#endif