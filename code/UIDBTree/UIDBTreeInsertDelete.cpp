#include "UIDBTree.h"

std::pair<UIDBTreeResultCode, UIDBNode*> UIDBTree::InsertNodeByKey(ByteVector key, ByteVector value)
{
    //Figure out where to insert the entry.
    UIDBNode* currentNode = rootNode.get();
    if (currentNode == nullptr)
    {
        //Create the root node.
        currentNode = new UIDBNode();
        currentNode->key = key;
        currentNode->values = ByteVectorVector({ value });
        rootNode.reset(currentNode);
        ++treeNodeCount;
        //No need for rebalancing.
        return { UIDBTreeResultCode::Success, currentNode };
    }
    else
    {
        //Compare the keys and navigate to one of the children. Repeat until an empty leaf node is found. Insert there.
        std::vector<UIDBNode*> traversalHistory;
        char comparisonResult;
        do
        {
            //Keep track of the traversal history down to the inserted node, for later rebalancing, if needed.
            traversalHistory.push_back(currentNode);
            //Compare the current node's key with the given key, to decide what to do next.
            comparisonResult = UIDBTree::compareKeys(currentNode->key, key);
            if (comparisonResult > 0)
            {
                //New key > current key; navigate to the right child.
                currentNode = currentNode->rightChildNode.get();
                if (currentNode == nullptr)
                {
                    //Now that the empty leaf node is found, insert the new node there.
                    currentNode = new UIDBNode();
                    currentNode->key = key;
                    currentNode->values = ByteVectorVector({ value });
                    currentNode->parentNode = traversalHistory.back();
                    traversalHistory.back()->rightChildNode.reset(currentNode);
                    ++treeNodeCount;
                    propagateBalanceChange(traversalHistory, currentNode, 1);
                    return { UIDBTreeResultCode::Success, currentNode };
                }
            }
            else if (comparisonResult < 0)
            {
                //New key < current key; navigate to the left child.
                currentNode = currentNode->leftChildNode.get();
                if (currentNode == nullptr)
                {
                    //Now that the empty leaf node is found, insert the new node there.
                    currentNode = new UIDBNode();
                    currentNode->key = key;
                    currentNode->values = ByteVectorVector({ value });
                    currentNode->parentNode = traversalHistory.back();
                    traversalHistory.back()->leftChildNode.reset(currentNode);
                    ++treeNodeCount;
                    propagateBalanceChange(traversalHistory, currentNode, 1);
                    return { UIDBTreeResultCode::Success, currentNode };
                }
            }
            else
            {
                //New key == current key; append duplicate value, if allowed.
                if (duplicatesAllowed)
                {
                    //Duplicates allowed; append the value.
                    currentNode->values.push_back(value);
                    //No need for rebalancing.
                    return { UIDBTreeResultCode::SuccessDuplicateValueAppended, currentNode };
                }
                else
                {
                    //Duplicates not allowed! Return the node that already exists with the given key. No need for
                    //rebalancing.
                    return { UIDBTreeResultCode::NoDuplicatesAllowedError, currentNode };
                }
            }
        }
        while (true);
    }
}

UIDBTreeResultCode UIDBTree::DeleteNodeByKey(ByteVector key)
{
    return UIDBTreeResultCode::Error;
}