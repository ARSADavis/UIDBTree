#pragma once

#include "../Globals.h"


//Use this to compute the parent balance when the child max depths are known.
static inline char GetMaxDepthBalance(char leftChildMaxDepth, char rightChildMaxDepth)
{
    return rightChildMaxDepth - leftChildMaxDepth;
}


//Use this to compute the parent max depth when the child max depths are known.
static inline char GetParentMaxDepth(char leftChildMaxDepth, char rightChildMaxDepth)
{
    return std::max(leftChildMaxDepth, rightChildMaxDepth) + 1;
}


//Use these when the parent max depth and balance are known, and when the divide between shallow vs. balanced or deep is
//known. Prefer these to the left/right functions because these specify and utilize more foreknowledge.
static inline char GetShallowChildMaxDepth(char parentMaxDepth, char parentMaxDepthBalance)
{
    return parentMaxDepth - 1 - std::abs(parentMaxDepthBalance);
}
static inline char GetBalancedDeepChildMaxDepth(char parentMaxDepth)
{
    return parentMaxDepth - 1;
}


//Use these when the parent max depth and balance are known, and when the divide between shallow vs. balanced or deep is
//not known. Prefer the shallow/balanced/deep functions to these because those specify and utilize more foreknowledge.
static inline char GetLeftChildMaxDepth(char parentMaxDepth, char parentMaxDepthBalance)
{
    return parentMaxDepth - 1 - std::max(parentMaxDepthBalance, char(0));
}
static inline char GetRightChildMaxDepth(char parentMaxDepth, char parentMaxDepthBalance)
{
    return parentMaxDepth - 1 + std::min(char(0), parentMaxDepthBalance);
}