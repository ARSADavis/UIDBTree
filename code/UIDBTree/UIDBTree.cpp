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

std::pair<UIDBTreeResultCode, UIDBNode*> UIDBTree::GetLowestNodeByKey(UIDBNode* topNode)
{
    if (topNode == nullptr)
    {
        return { UIDBTreeResultCode::NodeWasNullptrError, topNode };
    }
    UIDBNode* nextNode = topNode->leftChildNode.get();
    while (nextNode != nullptr)
    {
        topNode = nextNode;
        nextNode = nextNode->leftChildNode.get();
    }
    return { UIDBTreeResultCode::Success, topNode };
}
std::pair<UIDBTreeResultCode, std::vector<UIDBNode*>> UIDBTree::TraverseToLowestNodeByKey(
    UIDBNode* topNode, std::vector<UIDBNode*> startingTraversalHistory)
{
    std::vector<UIDBNode*> traversalHistory = startingTraversalHistory;
    if (topNode == nullptr)
    {
        return { UIDBTreeResultCode::NoNodeFound, traversalHistory };
    }
    UIDBNode* nextNode = topNode->leftChildNode.get();
    while (nextNode != nullptr)
    {
        traversalHistory.push_back(nextNode);
        topNode = nextNode;
        nextNode = nextNode->leftChildNode.get();
    }
    return { UIDBTreeResultCode::Success, traversalHistory };
}
std::pair<UIDBTreeResultCode, UIDBNode*> UIDBTree::GetHighestNodeByKey(UIDBNode* topNode)
{
    if (topNode == nullptr)
    {
        return { UIDBTreeResultCode::NodeWasNullptrError, topNode };
    }
    UIDBNode* nextNode = topNode->rightChildNode.get();
    while (nextNode != nullptr)
    {
        topNode = nextNode;
        nextNode = topNode->rightChildNode.get();
    }
    return { UIDBTreeResultCode::Success, topNode };
}
std::pair<UIDBTreeResultCode, std::vector<UIDBNode*>> UIDBTree::TraverseToHighestNodeByKey(
    UIDBNode* topNode, std::vector<UIDBNode*> startingTraversalHistory)
{
    std::vector<UIDBNode*> traversalHistory = startingTraversalHistory;
    if (topNode == nullptr)
    {
        return { UIDBTreeResultCode::NoNodeFound, traversalHistory };
    }
    UIDBNode* nextNode = topNode->rightChildNode.get();
    while (nextNode != nullptr)
    {
        traversalHistory.push_back(nextNode);
        topNode = nextNode;
        nextNode = topNode->rightChildNode.get();
    }
    return { UIDBTreeResultCode::Success, traversalHistory };
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

std::pair<UIDBTreeResultCode, std::vector<UIDBNode*>> UIDBTree::TraverseToNodeByKey(ByteVector key)
{
    std::vector<UIDBNode*> traversalHistory;
    char comparisonResult;
    UIDBNode* currentNode = rootNode.get();
    while (currentNode != nullptr)
    {
        traversalHistory.push_back(currentNode);
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
            return { UIDBTreeResultCode::Success, traversalHistory };
        }
    }
    return { UIDBTreeResultCode::NoNodeFound, traversalHistory };
}


//Returns the sign of the comparison of the two keys: second > first: 1, second < first: -1, second == first: 0.
char UIDBTree::compareKeys(ByteVector firstKey, ByteVector secondKey)
{
    return (secondKey > firstKey ? 1 : (secondKey < firstKey ? -1 : 0));
}