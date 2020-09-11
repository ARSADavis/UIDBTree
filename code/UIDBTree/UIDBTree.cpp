#include "UIDBTree.h"

UIDBTree::UIDBTree()
{

}

UIDBTree::~UIDBTree()
{

}

bool UIDBTree::IsEmpty()
{
    return rootNode.get() == nullptr;
}
observer_ptr<UIDBNode> UIDBTree::GetRootNode()
{
    return make_observer(rootNode.get());
}

//observer_ptr<UIDBNode> UIDBTree::GetLowestNodeByKey()
//{

//}
//observer_ptr<UIDBNode> UIDBTree::GetHighestNodeByKey()
//{

//}
std::pair<UIDBTreeResultCode, observer_ptr<UIDBNode>> UIDBTree::InsertNodeByKey(ByteVector key, ByteVector value)
{
    UIDBNode* newNode = new UIDBNode();
    newNode->SetKey(key);
    newNode->SetValue(value);
    rootNode.reset(newNode);
    return { UIDBTreeResultCode::Success, make_observer(newNode) };
}
//std::pair<UIDBTreeResultCode, std::vector<observer_ptr<UIDBNode>>> UIDBTree::FindNodesByKey()
//{

//}
//UIDBTreeResultCode UIDBTree::DeleteNodeByKey()
//{

//}