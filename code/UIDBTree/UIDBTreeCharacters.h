#pragma once

#include "../Globals.h"

#define UIDBTREESHORTPRINT

//Vertical.
//├─┬─┐ ┌─┬─┤
//╘═╤═╕ ╒═╤═╛
#ifdef UIDBTREESHORTPRINT
    static const std::wstring UIDBVTreeRightChild = L"╞";
    static const std::wstring UIDBVTreeLeftChild = L"└";
    static const std::wstring UIDBVTreeAncestorNext = L"│ ";
    static const std::wstring UIDBVTreeAncestorNoMore = L"  ";
#else
    static const std::wstring UIDBVTreeRightChild = L"╞═";
    static const std::wstring UIDBVTreeLeftChild = L"└─";
    static const std::wstring UIDBVTreeAncestorNext = L" │ ";
    static const std::wstring UIDBVTreeAncestorNoMore = L"   ";
#endif

//Horizontal.
//  ┌──┴──┐
//┌─┴─┐ ┌─┴─┐
static const wchar_t UIDBHTreeParent = L'┴';
static const wchar_t UIDBHTreeNext = L'─';
static const wchar_t UIDBHTreeEmpty = L' ';
static const wchar_t UIDBHTreeRightChild = L'┐';
static const wchar_t UIDBHTreeLeftChild = L'┌';