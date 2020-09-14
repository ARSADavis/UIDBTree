#include "UIDBNode.h"

UIDBNode::UIDBNode()
{

}


UIDBNode* UIDBNode::GetParentNode()
{
    return parentNode.get();
}
UIDBNode* UIDBNode::GetLeftChildNode()
{
    return leftChildNode.get();
}
UIDBNode* UIDBNode::GetRightChildNode()
{
    return rightChildNode.get();
}
UIDBNode* UIDBNode::GetNextNode()
{
    return nextNode.get();
}
UIDBNode* UIDBNode::GetPreviousNode()
{
    return previousNode.get();
}

void UIDBNode::SetParentNode(UIDBNode* parentNode)
{
    this->parentNode.reset(parentNode);
}
void UIDBNode::SetLeftChildNode(UIDBNode* leftChildNode)
{
    this->leftChildNode.reset(leftChildNode);
}
void UIDBNode::SetRightChildNode(UIDBNode* rightChildNode)
{
    this->rightChildNode.reset(rightChildNode);
}
void UIDBNode::SetNextNode(UIDBNode* nextNode)
{
    this->nextNode.reset(nextNode);
}
void UIDBNode::SetPreviousNode(UIDBNode* previousNode)
{
    this->previousNode.reset(previousNode);
}


const ByteVector UIDBNode::GetKey()
{
    return key;
}
const ByteVector UIDBNode::GetValue()
{
    return value;
}

void UIDBNode::SetKey(ByteVector key)
{
    this->key = key;
}
void UIDBNode::SetValue(ByteVector value)
{
    this->value = value;
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
        const ByteVector key = convertMe->GetKey();
        const ByteVector value = convertMe->GetValue();
        wss << L"{ \"" << BVToWString(key) << L"\", \"" << BVToWString(value) << L"\" }";
        return wss.str();
    }
}