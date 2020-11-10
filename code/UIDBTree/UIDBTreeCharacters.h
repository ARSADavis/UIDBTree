#pragma once

#include "../Globals.h"

#define UIDBTREESHORTPRINT

//Vertical.
//├─┬─┐ ┌─┬─┤
//╘═╤═╕ ╒═╤═╛
#ifdef UIDBTREESHORTPRINT
    static std::wstring UIDBVTreeRightChild = L"╞";
    static std::wstring UIDBVTreeLeftChild = L"└";
    static std::wstring UIDBVTreeAncestorNext = L"│ ";
    static std::wstring UIDBVTreeAncestorNoMore = L"  ";
#else
    static std::wstring UIDBVTreeRightChild = L"╞═";
    static std::wstring UIDBVTreeLeftChild = L"└─";
    static std::wstring UIDBVTreeAncestorNext = L" │ ";
    static std::wstring UIDBVTreeAncestorNoMore = L"   ";
#endif

//Horizontal.
//  ┌──┴──┐
//┌─┴─┐ ┌─┴─┐
static std::wstring UIDBHTreeParent = L"┴";
static std::wstring UIDBHTreeNext = L"─";
static std::wstring UIDBHTreeEmpty = L" ";
static std::wstring UIDBHTreeRightChild = L"┐";
static std::wstring UIDBHTreeLeftChild = L"┌";