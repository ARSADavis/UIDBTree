#include "UIDBTree.h"

UIDBTree::OperationResult* UIDBTree::InsertNodeByKey(ByteVector key, ByteVector value)
{
    //Figure out where to insert the entry.
    if (rootNode.get() == nullptr)
    {
        //Create the root node.
        UIDBNode* currentNode = new UIDBNode();
        currentNode->key = key;
        currentNode->values = ByteVectorVector({ value });
        rootNode.reset(currentNode);
        ++treeNodeCount;
        //No need for rebalancing.
        Result->HadError = false;
        Result->Node = currentNode;
        Result->DuplicateKey = false;
    }
    else
    {
        OperationResult* findResult = FindNodeOrNearbyByKey(key);
        UIDBNode* foundNode = findResult->Node;
        if (findResult->FoundExactNode)
        {
            //Exact match found. New key == lowest key; append duplicate value, if allowed. No need for rebalancing.
            if (duplicatesAllowed)
            {
                //Duplicates allowed; append the value.
                foundNode->values.push_back(value);
                Result->HadError = false;
            }
            else
            {
                //Duplicates not allowed! Return the node that already exists with the given key.
                Result->HadError = true;
            }
            Result->Node = foundNode;
            Result->DuplicateKey = true;
        }
        else
        {
            //No exact match found. Compare the lowest node's key with the given key, to decide what to do next.
            char comparisonResult = UIDBTree::compareKeys(foundNode->key, key);
            if (comparisonResult > 0)
            {
                //New key > lowest key; insert the new node as the right child.
                UIDBNode* currentNode = new UIDBNode();
                currentNode->key = key;
                currentNode->values = ByteVectorVector({ value });
                currentNode->parentNode = foundNode;
                foundNode->rightChildNode.reset(currentNode);
                ++treeNodeCount;
                propagateBalanceChange(foundNode, true, 1);
                Result->HadError = false;
                Result->Node = currentNode;
                Result->DuplicateKey = false;
            }
            else
            {
                //New key < lowest key; insert the new node as the left child.
                UIDBNode* currentNode = new UIDBNode();
                currentNode->key = key;
                currentNode->values = ByteVectorVector({ value });
                currentNode->parentNode = foundNode;
                foundNode->leftChildNode.reset(currentNode);
                ++treeNodeCount;
                propagateBalanceChange(foundNode, false, 1);
                Result->HadError = false;
                Result->Node = currentNode;
                Result->DuplicateKey = false;
            }
        }
    }
    return Result;
}

UIDBTree::OperationResult* UIDBTree::DeleteNodeByKey(ByteVector key)
{
    OperationResult* findResult = FindNodeOrNearbyByKey(key);

    //First see if there's a node to delete.
    if (!findResult->FoundExactNode)
    {
        //Nothing to delete.
        Result->HadError = true;
        Result->FoundExactNode = false;
        return Result;
    }

    UIDBNode* foundNode = findResult->Node;

    //Then, determine what to replace the deleted node with, based on its descendants, if any.
    if (foundNode->leftChildNode.get() == nullptr)
    {
        if (foundNode->rightChildNode.get() == nullptr)
        {
            //Node to be deleted is a leaf node. This is easiest; simply delete it and replace it with nullptr.
            if (rootNode.get() == foundNode)
            {
                //Root node.
                rootNode.reset(nullptr);
            }
            else
            {
                //Non-root node.
                UIDBNode* parentNode = foundNode->parentNode;
                if (foundNode == parentNode->rightChildNode.get())
                {
                    //Right child.
                    parentNode->rightChildNode.reset(nullptr);
                    propagateBalanceChange(parentNode, true, -1);
                }
                else
                {
                    //Left child.
                    parentNode->leftChildNode.reset(nullptr);
                    propagateBalanceChange(parentNode, false, -1);
                }
            }
        }
        else
        {
            //Node to be deleted has a right child only. Simply delete it and replace it with its child. As a shortcut,
            //instead copying the key and value over to replace.
            foundNode->key = foundNode->rightChildNode->key;
            foundNode->values = foundNode->rightChildNode->values;
            foundNode->rightChildNode.release();
            propagateBalanceChange(foundNode, true, -1);
        }
    }
    else if (foundNode->rightChildNode.get() == nullptr)
    {
        //Node to be deleted has a left child only. Simply delete it and replace it with its child. As a shortcut,
        //copying the key and value over to replace.
        foundNode->key = foundNode->leftChildNode->key;
        foundNode->values = foundNode->leftChildNode->values;
        foundNode->leftChildNode.release();
        propagateBalanceChange(foundNode, false, -1);
    }
    else
    {
        //Node to be deleted has both left and right children. Delete it and replace it with its inorder
        //(ordered-immediate) neighbor. Whether inorder successor vs. predecessor is used, it will work (it will be a
        //leaf and will not disrupt the ordering of nodes when moved); however, in order to minimize rebalancing, the
        //replacement will be pulled from the deeper side, if either.
        if (foundNode->subtreeMaxDepthBalance >= 0)
        {
            //Balanced or right deep; node to be deleted is the inorder successor, which is the lowest descendant of the
            //right child. As a shortcut, copying the key and value over to replace.
            OperationResult* lowestResult = GetLowestNodeByKey(foundNode->rightChildNode.get());
            foundNode->key = lowestResult->Node->key;
            foundNode->values = lowestResult->Node->values;
            //There are 2 cases here: Either the inorder successor is the right child itself, or the inorder successor
            //is a left descendant of the right child.
            if (lowestResult->Node == foundNode->rightChildNode.get())
            {
                //Inorder successor is the right child itself.
                foundNode->rightChildNode.reset(nullptr);
                propagateBalanceChange(foundNode, true, -1);
            }
            else
            {
                //Inorder successor is a left descendant of the right child.
                UIDBNode* inorderSuccessorParentNode = lowestResult->Node->parentNode;
                inorderSuccessorParentNode->leftChildNode.reset(nullptr);
                propagateBalanceChange(inorderSuccessorParentNode, false, -1);
            }
        }
        else
        {
            //Left deep; note to be deleted is the inorder predecessor, which is the highest descendant of the left
            //child. As a shortcut, copying the key and value over to replace.
            OperationResult* highestResult = GetHighestNodeByKey(foundNode->leftChildNode.get());
            foundNode->key = highestResult->Node->key;
            foundNode->values = highestResult->Node->values;
            //There are 2 cases here: Either the inorder predecessor is the left child itself, or the inorder
            //predecessor is a right descendant of the left child.
            if (highestResult->Node == foundNode->leftChildNode.get())
            {
                //Inorder successor is the left child itself.
                foundNode->leftChildNode.reset(nullptr);
                propagateBalanceChange(foundNode, false, -1);
            }
            else
            {
                //Inorder predecessor is a right descendant of the left child.
                UIDBNode* inorderPredecessorParentNode = highestResult->Node->parentNode;
                inorderPredecessorParentNode->rightChildNode.reset(nullptr);
                propagateBalanceChange(inorderPredecessorParentNode, true, -1);
            }
        }
    }
    --treeNodeCount;
    Result->HadError = false;
    Result->FoundExactNode = true;
    return Result;
}