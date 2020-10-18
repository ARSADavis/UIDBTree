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


std::wstring UIDBNode::ToWString(UIDBNode* convertMe)
{
    if (convertMe == nullptr)
    {
        return L"<nullptr>";
    }
    else
    {
        std::wstringstream wss;
        wss << L"(" << (int)convertMe->subtreeMaxDepthBalance << ") " << BVToT<TreeKeyType>(convertMe->key);
        wss << L": \"" << BVVToWString(convertMe->values) << L"\"";
        return wss.str();
    }
}