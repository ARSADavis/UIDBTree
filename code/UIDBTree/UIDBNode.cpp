#include "UIDBNode.h"

UIDBNode::UIDBNode(): subtreeMaxDepthBalance(0)
{

}


UIDBNode* UIDBNode::GetParentNode()
{
    return parentNode;
}
UIDBNode* UIDBNode::GetLeftChildNode()
{
    return leftChildNode.get();
}
UIDBNode* UIDBNode::GetRightChildNode()
{
    return rightChildNode.get();
}
//UIDBNode* UIDBNode::GetNextNode()
//{
//    return nextNode.get();
//}
//UIDBNode* UIDBNode::GetPreviousNode()
//{
//    return previousNode.get();
//}


const ByteVector UIDBNode::GetKey()
{
    return key;
}
const ByteVectorVector UIDBNode::GetValues()
{
    return values;
}


std::wstring UIDBNode::ToWString(UIDBNode* convertMe, bool compact, bool includeValues)
{
    if (convertMe == nullptr)
    {
        if (compact)
        {
            return L"<span class=\"leaf\">&lt;L&gt;</span>";
        }
        else
        {
            return L"<span class=\"leaf\">&lt;leaf&gt;</span>";
        }
    }
    else
    {
        std::wstringstream wss;
        if (convertMe->subtreeMaxDepthBalance < 0)
        {
            if (convertMe->subtreeMaxDepthBalance == -1)
            {
                wss << L"<span class=\"gba\">";
            }
            else
            {
                wss << L"<span class=\"bba\">";
            }
            for (char arrowCount = 0; arrowCount > convertMe->subtreeMaxDepthBalance; --arrowCount)
            {
                wss << L"&lt;";
            }
            wss << L"</span>";
            wss << L" ";
        }
        wss << L"<span class=\"key\">" << BVToT<TreeKeyType>(convertMe->key) << L"</span>";
        if (convertMe->subtreeMaxDepthBalance > 0)
        {
            wss << L" ";
            if (convertMe->subtreeMaxDepthBalance == 1)
            {
                wss << L"<span class=\"gba\">";
            }
            else
            {
                wss << L"<span class=\"bba\">";
            }
            for (char arrowCount = 0; arrowCount < convertMe->subtreeMaxDepthBalance; ++arrowCount)
            {
                wss << L"&gt;";
            }
            wss << L"</span>";
        }
        if (includeValues)
        {
            wss << L": <span class=\"val\">\"" << BVVToWString(convertMe->values) << L"\"</span>";
        }
        return wss.str();
    }
}