#include "UIDBTree.h"
#include <iostream>

UIDBTree::UIDBTree(bool duplicatesAllowed)
{
    this->duplicatesAllowed = duplicatesAllowed;
    Result = new OperationResult();
    treeNodeCount = 0;
}

UIDBTree::~UIDBTree()
{
    //Recursively delete all nodes via. RAII deconstruction.
    if (rootNode.get() != nullptr)
    {
        rootNode.reset();
    }
    if (Result != nullptr)
    {
        delete Result;
        Result = nullptr;
    }
}

bool UIDBTree::IsEmpty()
{
    return (rootNode.get() == nullptr);
}
UIDBNode* UIDBTree::GetRootNode()
{
    return rootNode.get();
}

UIDBTree::OperationResult* UIDBTree::GetMaxDepth()
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
    Result->HadError = false;
    Result->Depth = maxDepth;
    return Result;
}

UIDBTree::OperationResult* UIDBTree::GetLowestNodeByKey(UIDBNode* topNode)
{
    Result->HadError = false;
    if (topNode == nullptr)
    {
        Result->Node = nullptr;
        return Result;
    }
    UIDBNode* nextNode = topNode->leftChildNode.get();
    while (nextNode != nullptr)
    {
        topNode = nextNode;
        nextNode = nextNode->leftChildNode.get();
    }
    Result->Node = topNode;
    return Result;
}
UIDBTree::OperationResult* UIDBTree::GetHighestNodeByKey(UIDBNode* topNode)
{
    Result->HadError = false;
    if (topNode == nullptr)
    {
        Result->Node = nullptr;
        return Result;
    }
    UIDBNode* nextNode = topNode->rightChildNode.get();
    while (nextNode != nullptr)
    {
        topNode = nextNode;
        nextNode = topNode->rightChildNode.get();
    }
    Result->Node = topNode;
    return Result;
}

UIDBTree::OperationResult* UIDBTree::FindNodeOrNearbyByKey(ByteVector key)
{
    Result->HadError = false;
    Result->Node = nullptr;
    char comparisonResult;
    UIDBNode* currentNode = rootNode.get();
    while (currentNode != nullptr)
    {
        Result->Node = currentNode;
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
            Result->FoundExactNode = true;
            return Result;
        }
    }
    Result->FoundExactNode = false;
    return Result;
}


//Returns the sign of the comparison of the two keys: second > first: 1, second < first: -1, second == first: 0.
char UIDBTree::compareKeys(ByteVector firstKey, ByteVector secondKey)
{
    return (secondKey > firstKey ? 1 : (secondKey < firstKey ? -1 : 0));
}