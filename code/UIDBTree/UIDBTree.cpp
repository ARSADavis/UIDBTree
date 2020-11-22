#include "UIDBTree.h"

UIDBTree::UIDBTree(bool duplicatesAllowed)
{
    this->duplicatesAllowed = duplicatesAllowed;
    treeNodeCount = 0;
}

UIDBTree::~UIDBTree()
{

}

bool UIDBTree::IsEmpty()
{
    return rootNode.get() == nullptr;
}
UIDBNode* UIDBTree::GetRootNode()
{
    return rootNode.get();
}
unsigned char UIDBTree::GetMaxDepth()
{
    unsigned char maxDepth = 0;
    UIDBNode* currentNode = rootNode.get();
    while (currentNode != nullptr)
    {
        ++maxDepth;
        if (currentNode->subtreeMaxDepthBalance <= 0)
        {
            currentNode = currentNode->leftChildNode.get();
        }
        else
        {
            currentNode = currentNode->rightChildNode.get();
        }
    }
    return maxDepth;
}

//UIDBNode* UIDBTree::GetLowestNodeByKey()
//{

//}
//UIDBNode* UIDBTree::GetHighestNodeByKey()
//{

//}

//std::pair<UIDBTreeResultCode, std::vector<UIDBNode*>> UIDBTree::FindNodesByKey()
//{

//}


//Returns the sign of the comparison of the two keys: second > first: 1, second < first: -1, second == first: 0.
char UIDBTree::compareKeys(ByteVector firstKey, ByteVector secondKey)
{
    return (secondKey > firstKey ? 1 : (secondKey < firstKey ? -1 : 0));
}