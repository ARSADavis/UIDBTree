#include "UIDBTree.h"

std::pair<UIDBTreeResultCode, UIDBNode*> UIDBTree::InsertNodeByKey(ByteVector key, ByteVector value)
{
    //Figure out where to insert the entry.
    std::pair<UIDBTreeResultCode, std::vector<UIDBNode*>> traversalResult = TraverseToNodeByKey(key);
    std::vector<UIDBNode*> traversalHistory = traversalResult.second;
    if (traversalHistory.empty())
    {
        //Create the root node.
        UIDBNode* currentNode = new UIDBNode();
        currentNode->key = key;
        currentNode->values = ByteVectorVector({ value });
        rootNode.reset(currentNode);
        ++treeNodeCount;
        //No need for rebalancing.
        return { UIDBTreeResultCode::Success, currentNode };
    }
    else
    {
        //Compare the lowest node's key with the given key, to decide what to do next.
        UIDBNode* deepestTraversalNode = traversalHistory.back();
        char comparisonResult = UIDBTree::compareKeys(deepestTraversalNode->key, key);
        if (comparisonResult > 0)
        {
            //New key > lowest key; insert the new node as the right child.
            UIDBNode* currentNode = new UIDBNode();
            currentNode->key = key;
            currentNode->values = ByteVectorVector({ value });
            currentNode->parentNode = deepestTraversalNode;
            deepestTraversalNode->rightChildNode.reset(currentNode);
            ++treeNodeCount;
            propagateBalanceChange(traversalHistory, true, 1);
            return { UIDBTreeResultCode::Success, currentNode };
        }
        else if (comparisonResult < 0)
        {
            //New key < lowest key; insert the new node as the left child.
            UIDBNode* currentNode = new UIDBNode();
            currentNode->key = key;
            currentNode->values = ByteVectorVector({ value });
            currentNode->parentNode = deepestTraversalNode;
            deepestTraversalNode->leftChildNode.reset(currentNode);
            ++treeNodeCount;
            propagateBalanceChange(traversalHistory, false, 1);
            return { UIDBTreeResultCode::Success, currentNode };
        }
        else
        {
            //New key == lowest key; append duplicate value, if allowed.
            if (duplicatesAllowed)
            {
                //Duplicates allowed; append the value.
                deepestTraversalNode->values.push_back(value);
                //No need for rebalancing.
                return { UIDBTreeResultCode::SuccessDuplicateValueAppended, deepestTraversalNode };
            }
            else
            {
                //Duplicates not allowed! Return the node that already exists with the given key. No need for
                //rebalancing.
                return { UIDBTreeResultCode::NoDuplicatesAllowedError, deepestTraversalNode };
            }
        }
    }
}

UIDBTreeResultCode UIDBTree::DeleteNodeByKey(ByteVector key)
{
    return UIDBTreeResultCode::Error;
}