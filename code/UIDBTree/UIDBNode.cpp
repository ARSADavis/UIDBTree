#include "UIDBNode.h"

UIDBNode::UIDBNode()
{

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