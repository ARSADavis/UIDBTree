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

UIDBNode* UIDBTree::GetLowestNodeByKey(UIDBNode* topNode)
{
    if (topNode == nullptr)
    {
        return topNode;
    }
    UIDBNode* nextNode = topNode->leftChildNode.get();
    while (nextNode != nullptr)
    {
        topNode = nextNode;
        nextNode = nextNode->leftChildNode.get();
    }
    return topNode;
}
UIDBNode* UIDBTree::GetHighestNodeByKey(UIDBNode* topNode)
{
    if (topNode == nullptr)
    {
        return topNode;
    }
    UIDBNode* nextNode = topNode->rightChildNode.get();
    while (nextNode != nullptr)
    {
        topNode = nextNode;
        nextNode = topNode->rightChildNode.get();
    }
    return topNode;
}

std::pair<UIDBTreeResultCode, UIDBNode*> UIDBTree::FindNodeByKey(ByteVector key)
{
    char comparisonResult;
    UIDBNode* currentNode = rootNode.get();
    while (currentNode != nullptr)
    {
        //Compare the current node's key with the given key, to decide what to do next.
        comparisonResult = UIDBTree::compareKeys(currentNode->key, key);
        if (comparisonResult > 0)
        {
            //Search key > current key; navigate to the right child.
            currentNode = currentNode->rightChildNode.get();
        }
        else if (comparisonResult < 0)
        {
            //Search key < current key; navigate to the left child.
            currentNode = currentNode->leftChildNode.get();
        }
        else
        {
            //Search key == current key; found it!
            return { UIDBTreeResultCode::Success, currentNode };
        }
    }
    return { UIDBTreeResultCode::NoNodeFound, nullptr };
}


//Returns the sign of the comparison of the two keys: second > first: 1, second < first: -1, second == first: 0.
char UIDBTree::compareKeys(ByteVector firstKey, ByteVector secondKey)
{
    return (secondKey > firstKey ? 1 : (secondKey < firstKey ? -1 : 0));
}