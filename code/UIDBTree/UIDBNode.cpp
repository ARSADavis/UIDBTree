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


std::wstring UIDBNode::ToWString(UIDBNode* convertMe, TreePrintingTypes treePrintingType)
{
    std::wstringstream wss;
    if (convertMe == nullptr)
    {
        switch (treePrintingType)
        {
            case TreePrintingTypes::HorizontalHTML:
                wss << L"<span class=\"leaf\">" << PadCentered(L"&lt;L&gt;", L' ', 5 + 6) << L"</span>";
                break;
            case TreePrintingTypes::VerticalHTML:
                wss << L"<span class=\"leaf\">&lt;leaf&gt;</span>";
                break;
        }
    }
    else
    {
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
            wss << L"<span class=\"key\">";
            wss << PadTheRight(std::to_wstring(BVToT<TreeKeyType>(convertMe->key)),
                L' ', 5 - 1 + convertMe->subtreeMaxDepthBalance);
            wss << L"</span>";
        }
        else if (convertMe->subtreeMaxDepthBalance > 0)
        {
            wss << L"<span class=\"key\">";
            wss << PadTheLeft(std::to_wstring(BVToT<TreeKeyType>(convertMe->key)),
                L' ', 5 - 1 - convertMe->subtreeMaxDepthBalance);
            wss << L"</span>";
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
        else
        {
            wss << L"<span class=\"key\">";
            wss << PadCentered(std::to_wstring(BVToT<TreeKeyType>(convertMe->key)), L' ', 5);
            wss << L"</span>";
        }
        if (treePrintingType == TreePrintingTypes::VerticalHTML)
        {
            wss << L": <span class=\"val\">\"" << BVVToWString(convertMe->values) << L"\"</span>";
        }
    }
    return wss.str();
}