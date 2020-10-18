#include "UIDBTree.h"
#include "UIDBTreeCharacters.h"
#include "UIDBTreeBalancing.h"



#include <iostream>

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

//UIDBNode* UIDBTree::GetLowestNodeByKey()
//{

//}
//UIDBNode* UIDBTree::GetHighestNodeByKey()
//{

//}
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
                //New key > current key.

                //Navigate to the right child.
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
                //New key < current key.

                //Navigate to the left child.
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
//std::pair<UIDBTreeResultCode, std::vector<UIDBNode*>> UIDBTree::FindNodesByKey()
//{

//}
//UIDBTreeResultCode UIDBTree::DeleteNodeByKey()
//{

//}


std::wstring UIDBTree::ToWString(UIDBTree* convertMe)
{
    if (convertMe == nullptr)
    {
        return L"<nullptr>";
    }
    else
    {
        UIDBNode* startingNode = convertMe->GetRootNode();
        if (startingNode == nullptr)
        {
            return L"<nullptr>";
        }
        std::wstringstream wss;
        wss << UIDBNode::ToWString(startingNode) << L"\n";
        std::vector<std::wstring> startingCharacters;
        UIDBTree::treeNodeToStringRecursive(wss, startingCharacters, startingNode);
        return wss.str();
    }
}



void UIDBTree::propagateBalanceChange(const std::vector<UIDBNode*>& traversalHistory, UIDBNode* lowestNode,
    char propagatingChange)
{
    //Apply any balance changes, starting with the parent of the node and working up to the root node. Check for the
    //need to rebalance each of the subtrees along the way.
    UIDBNode* childNode = lowestNode;
    char previousBalance;
    UIDBNode* primaryParentNode;
    UIDBNode* newPrimaryNode, * secondaryNode, * tertiaryNode, * quaternaryLeftNode, * quaternaryRightNode;
    bool rebalancing;
    for (UIDBNode* primaryNode: std::ranges::reverse_view(traversalHistory))
    {
        previousBalance = primaryNode->subtreeMaxDepthBalance;
        if (childNode == primaryNode->rightChildNode.get())
        {
            //Child node is right child.
            primaryNode->subtreeMaxDepthBalance += propagatingChange;
            if (propagatingChange > 0)
            {
                //Propagating an increase in depth balance. If the previous balance was less than 0, or left-deep, then
                //the subtree's max depth has not changed, because it is becoming more balanced.
                if (previousBalance < 0)
                {
                    //Stop propagating balance changes up the ancestry.
                    return;
                }
            }
            else
            {
                //Propagating a decrease in depth balance. If the previous balance was less than or equal to 0, or
                //balanced/left-deep, then the subtree's max depth has not changed, because it is becoming more
                //balanced.
                if (previousBalance <= 0)
                {
                    //Stop propagating balance changes up the ancestry.
                    return;
                }
            }
        }
        else
        {
            //Child node is left child.
            primaryNode->subtreeMaxDepthBalance -= propagatingChange;
            if (propagatingChange > 0)
            {
                //Propagating an increase in depth balance. If the previous balance was greater than 0, or right-deep,
                //then the subtree's max depth has not changed, because it is becoming more balanced.
                if (previousBalance > 0)
                {
                    //Stop propagating balance changes up the ancestry.
                    return;
                }
            }
            else
            {
                //Propagating a decrease in depth balance. If the previous balance was greater than or equal to 0, or
                //balanced/right-deep, then the subtree's max depth has not changed, because it is becoming more
                //balanced.
                if (previousBalance >= 0)
                {
                    //Stop propagating balance changes up the ancestry.
                    return;
                }
            }
        }
        //Check for a large imbalance, and rebalance if necessary.
        rebalancing = false;
        if (primaryNode->subtreeMaxDepthBalance > 1)
        {
            //Right subtree max depth is too deep compared to left subtree:
            //    Primary
            //                                                                         Secondary (Right)
            //                                  Tertiary (Left)
            //               Quaternary Left                       Quaternary Right
            primaryParentNode = primaryNode->parentNode;
            secondaryNode = primaryNode->rightChildNode.get();
            tertiaryNode = secondaryNode->leftChildNode.get();

            //Some calculations necessary for rebalancing. Note that these max depths are relative to the primary's max
            //depth, starting at 0.
            //The secondary level was right deep.
            char secondaryLeftOldMaxDepth = GetShallowChildMaxDepth(0, primaryNode->subtreeMaxDepthBalance);
            char secondaryRightOldMaxDepth = GetBalancedDeepChildMaxDepth(0);
            if (secondaryNode->subtreeMaxDepthBalance >= 0)
            {
                //The tertiary level is either balanced or right deep, meaning that a single rotation will do the trick.
                //For a single rotation, the secondary node becomes the primary node:
                //                                                                     Secondary (Right)
                //Primary
                //                              Tertiary (Left)
                //           Quaternary Left                       Quaternary Right
                newPrimaryNode = secondaryNode;
                secondaryNode->leftChildNode.release();
                secondaryNode->leftChildNode.reset(primaryNode);
                primaryNode->parentNode = secondaryNode;
                primaryNode->rightChildNode.release();
                primaryNode->rightChildNode.reset(tertiaryNode);
                if (tertiaryNode != nullptr)
                {
                    tertiaryNode->parentNode = primaryNode;
                }

                //Some calculations necessary for rebalancing. Note that these max depths are relative to the primary's
                //max depth, starting at 0.
                //The tertiary level was either balanced or right deep, since this is a single rotation.
                char tertiaryLeftOldMaxDepth =
                    GetLeftChildMaxDepth(secondaryRightOldMaxDepth, secondaryNode->subtreeMaxDepthBalance);
                char tertiaryRightMaxDepth = GetBalancedDeepChildMaxDepth(secondaryRightOldMaxDepth);
                //The primary's new max depth is simply the max of its childrens' max depths.
                char primaryNewMaxDepth = GetParentMaxDepth(secondaryLeftOldMaxDepth, tertiaryLeftOldMaxDepth);
                //The secondary right's new max depth is simply the max of its childrens' max depths.
                char secondaryRightNewMaxDepth = GetParentMaxDepth(primaryNewMaxDepth, tertiaryRightMaxDepth);

                //Max depth balance is the right child's max depth minus the left child's max depth.
                secondaryNode->subtreeMaxDepthBalance =
                    GetMaxDepthBalance(primaryNewMaxDepth, tertiaryRightMaxDepth);
                primaryNode->subtreeMaxDepthBalance =
                    GetMaxDepthBalance(secondaryLeftOldMaxDepth, tertiaryLeftOldMaxDepth);

                //After a rebalance, the propagation always becomes a decrease in depth balance.
                propagatingChange = -secondaryRightNewMaxDepth - 1;
            }
            else
            {
                //The tertiary level is left deep, meaning that a double rotation is necessary in order to restore
                //balance. This is because the problem subtree is the tertiary node, which in a single rotation would
                //merely be moved to the other side. For a double rotation, the tertiary node becomes the primary node:
                //                              Tertiary (Left)
                //Primary                                                              Secondary (Right)
                //           Quaternary Left                       Quaternary Right
                quaternaryLeftNode = tertiaryNode->leftChildNode.get();
                quaternaryRightNode = tertiaryNode->rightChildNode.get();
                newPrimaryNode = tertiaryNode;
                tertiaryNode->leftChildNode.release();
                tertiaryNode->leftChildNode.reset(primaryNode);
                primaryNode->parentNode = tertiaryNode;
                tertiaryNode->rightChildNode.release();
                tertiaryNode->rightChildNode.reset(secondaryNode);
                secondaryNode->parentNode = tertiaryNode;
                primaryNode->rightChildNode.release();
                primaryNode->rightChildNode.reset(quaternaryLeftNode);
                if (quaternaryLeftNode != nullptr)
                {
                    quaternaryLeftNode->parentNode = secondaryNode;
                }
                secondaryNode->leftChildNode.release();
                secondaryNode->leftChildNode.reset(quaternaryRightNode);
                if (quaternaryRightNode != nullptr)
                {
                    quaternaryRightNode->parentNode = secondaryNode;
                }

                //Some calculations necessary for rebalancing. Note that these max depths are relative to the primary's
                //max depth, starting at 0.
                //The tertiary level was left deep, since this is a double rotation.
                char tertiaryLeftOldMaxDepth = GetBalancedDeepChildMaxDepth(secondaryRightOldMaxDepth);
                char tertiaryRightMaxDepth =
                    GetShallowChildMaxDepth(secondaryRightOldMaxDepth, secondaryNode->subtreeMaxDepthBalance);
                //Quaternary max depths don't change in the rotation.
                char quaternaryLeftMaxDepth =
                    GetLeftChildMaxDepth(tertiaryLeftOldMaxDepth, tertiaryNode->subtreeMaxDepthBalance);
                char quaternaryRightMaxDepth =
                        GetRightChildMaxDepth(tertiaryLeftOldMaxDepth, tertiaryNode->subtreeMaxDepthBalance);
                //These nodes' new max depths are simply the max of their childrens' max depths.
                char primaryNewMaxDepth = GetParentMaxDepth(secondaryLeftOldMaxDepth, quaternaryLeftMaxDepth);
                char secondaryRightNewMaxDepth = GetParentMaxDepth(quaternaryRightMaxDepth, tertiaryRightMaxDepth);
                char tertiaryLeftNewMaxDepth = GetParentMaxDepth(primaryNewMaxDepth, secondaryRightNewMaxDepth);

                //Max depth balance is the right child's max depth minus the left child's max depth.
                tertiaryNode->subtreeMaxDepthBalance =
                    GetMaxDepthBalance(primaryNewMaxDepth, secondaryRightNewMaxDepth);
                secondaryNode->subtreeMaxDepthBalance =
                    GetMaxDepthBalance(quaternaryRightMaxDepth, tertiaryRightMaxDepth);
                primaryNode->subtreeMaxDepthBalance =
                    GetMaxDepthBalance(secondaryLeftOldMaxDepth, quaternaryLeftMaxDepth);

                //After a rebalance, the propagation always becomes a decrease in depth balance.
                propagatingChange = -tertiaryLeftNewMaxDepth - 1;
            }
            rebalancing = true;
        }
        else if (primaryNode->subtreeMaxDepthBalance < -1)
        {
            //Left subtree max depth is too deep compared to right subtree:
            //                                                                                   Primary
            //    Secondary (Left)
            //                                           Tertiary (Right)
            //                        Quaternary Left                        Quaternary Right
            primaryParentNode = primaryNode->parentNode;
            secondaryNode = primaryNode->leftChildNode.get();
            tertiaryNode = secondaryNode->rightChildNode.get();

            //Some calculations necessary for rebalancing. Note that these max depths are relative to the primary's max
            //depth, starting at 0.
            //The secondary level was left deep.
            char secondaryLeftOldMaxDepth = GetBalancedDeepChildMaxDepth(0);
            char secondaryRightOldMaxDepth = GetShallowChildMaxDepth(0, primaryNode->subtreeMaxDepthBalance);
            if (secondaryNode->subtreeMaxDepthBalance <= 0)
            {
                //The tertiary level is either balanced or left deep, meaning that a single rotation will do the trick.
                //For a single rotation, the secondary node becomes the primary node:
                //Secondary (Left)
                //                                                                               Primary
                //                                       Tertiary (Right)
                //                    Quaternary Left                        Quaternary Right
                newPrimaryNode = secondaryNode;
                secondaryNode->rightChildNode.release();
                secondaryNode->rightChildNode.reset(primaryNode);
                primaryNode->parentNode = secondaryNode;
                primaryNode->leftChildNode.release();
                primaryNode->leftChildNode.reset(tertiaryNode);
                if (tertiaryNode != nullptr)
                {
                    tertiaryNode->parentNode = primaryNode;
                }

                //Some calculations necessary for rebalancing. Note that these max depths are relative to the primary's
                //max depth, starting at 0.
                //The tertiary level was either balanced or left deep, since this is a single rotation.
                char tertiaryLeftMaxDepth = GetBalancedDeepChildMaxDepth(secondaryLeftOldMaxDepth);
                char tertiaryRightOldMaxDepth =
                    GetRightChildMaxDepth(secondaryLeftOldMaxDepth, secondaryNode->subtreeMaxDepthBalance);
                //The primary's new max depth is simply the max of its childrens' max depths.
                char primaryNewMaxDepth = GetParentMaxDepth(tertiaryRightOldMaxDepth, secondaryRightOldMaxDepth);
                //The secondary left's new max depth is simply the max of its childrens' max depths.
                char secondaryLeftNewMaxDepth = GetParentMaxDepth(tertiaryLeftMaxDepth, primaryNewMaxDepth);

                //Max depth balance is the right child's max depth minus the left child's max depth.
                secondaryNode->subtreeMaxDepthBalance =
                    GetMaxDepthBalance(tertiaryLeftMaxDepth, primaryNewMaxDepth);
                primaryNode->subtreeMaxDepthBalance =
                    GetMaxDepthBalance(tertiaryRightOldMaxDepth, secondaryRightOldMaxDepth);

                //After a rebalance, the propagation always becomes a decrease in depth balance.
                propagatingChange = -secondaryLeftNewMaxDepth - 1;
            }
            else
            {
                //The tertiary level is right deep, meaning that a double rotation is necessary in order to restore
                //balance. This is because the problem subtree is the tertiary node, which in a single rotation would
                //merely be moved to the other side. For a double rotation, the tertiary node becomes the primary node:
                //                                       Tertiary (Right)
                //Secondary (Left)                                                               Primary
                //                    Quaternary Left                        Quaternary Right
                quaternaryLeftNode = tertiaryNode->leftChildNode.get();
                quaternaryRightNode = tertiaryNode->rightChildNode.get();
                newPrimaryNode = tertiaryNode;
                tertiaryNode->leftChildNode.release();
                tertiaryNode->leftChildNode.reset(secondaryNode);
                secondaryNode->parentNode = tertiaryNode;
                tertiaryNode->rightChildNode.release();
                tertiaryNode->rightChildNode.reset(primaryNode);
                primaryNode->parentNode = tertiaryNode;
                secondaryNode->rightChildNode.release();
                secondaryNode->rightChildNode.reset(quaternaryLeftNode);
                if (quaternaryLeftNode != nullptr)
                {
                    quaternaryLeftNode->parentNode = secondaryNode;
                }
                primaryNode->leftChildNode.release();
                primaryNode->leftChildNode.reset(quaternaryRightNode);
                if (quaternaryRightNode != nullptr)
                {
                    quaternaryRightNode->parentNode = primaryNode;
                }

                //Some calculations necessary for rebalancing. Note that these max depths are relative to the primary's
                //max depth, starting at 0.
                //The tertiary level was right deep, since this is a double rotation.
                char tertiaryLeftMaxDepth =
                    GetShallowChildMaxDepth(secondaryLeftOldMaxDepth, secondaryNode->subtreeMaxDepthBalance);
                char tertiaryRightOldMaxDepth = GetBalancedDeepChildMaxDepth(secondaryLeftOldMaxDepth);
                //Quaternary max depths don't change in the rotation.
                char quaternaryLeftMaxDepth =
                    GetLeftChildMaxDepth(tertiaryRightOldMaxDepth, tertiaryNode->subtreeMaxDepthBalance);
                char quaternaryRightMaxDepth =
                    GetRightChildMaxDepth(tertiaryRightOldMaxDepth, tertiaryNode->subtreeMaxDepthBalance);
                //These nodes' new max depths are simply the max of their childrens' max depths.
                char primaryNewMaxDepth = GetParentMaxDepth(quaternaryRightMaxDepth, secondaryRightOldMaxDepth);
                char secondaryLeftNewMaxDepth = GetParentMaxDepth(tertiaryLeftMaxDepth, quaternaryLeftMaxDepth);
                char tertiaryRightNewMaxDepth = GetParentMaxDepth(secondaryLeftNewMaxDepth, primaryNewMaxDepth);

                //Max depth balance is the right child's max depth minus the left child's max depth.
                tertiaryNode->subtreeMaxDepthBalance =
                    GetMaxDepthBalance(secondaryLeftNewMaxDepth, primaryNewMaxDepth);
                secondaryNode->subtreeMaxDepthBalance =
                    GetMaxDepthBalance(tertiaryLeftMaxDepth, quaternaryLeftMaxDepth);
                primaryNode->subtreeMaxDepthBalance =
                    GetMaxDepthBalance(quaternaryRightMaxDepth, secondaryRightOldMaxDepth);

                //After a rebalance, the propagation always becomes a decrease in depth balance.
                propagatingChange = -tertiaryRightNewMaxDepth - 1;
            }
            rebalancing = true;
        }
        if (rebalancing)
        {
            //Update the old primary node's parent as well, if any.
            if (primaryParentNode == nullptr)
            {
                //Old primary node was root.
                rootNode.release();
                rootNode.reset(newPrimaryNode);
            }
            else if (primaryNode == primaryParentNode->rightChildNode.get())
            {
                //Old primary node was right child.
                primaryParentNode->rightChildNode.release();
                primaryParentNode->rightChildNode.reset(newPrimaryNode);
                newPrimaryNode->parentNode = primaryParentNode;
                //Balance change will be handled at the next level up, via. propagatingChange, if necessary.
            }
            else
            {
                //Old primary node was left child.
                primaryParentNode->leftChildNode.release();
                primaryParentNode->leftChildNode.reset(newPrimaryNode);
                newPrimaryNode->parentNode = primaryParentNode;
                //Balance change will be handled at the next level up, via. propagatingChange, if necessary.
            }
            if (propagatingChange == 0)
            {
                //Stop propagating balance changes up the ancestry.
                return;
            }
        }
        else
        {
            //No rebalance, and propagation continues as before.
            childNode = primaryNode;
        }
    }
}


//Returns the sign of the comparison of the two keys: second > first: 1, second < first: -1, second == first: 0.
char UIDBTree::compareKeys(ByteVector firstKey, ByteVector secondKey)
{
    return (secondKey > firstKey ? 1 : (secondKey < firstKey ? -1 : 0));
}

void UIDBTree::treeNodeToStringRecursive(std::wstringstream& wss, std::vector<std::wstring>& startingCharacters,
    UIDBNode* convertMe)
{
    static char recursionCount = 0;
    ++recursionCount;
    if (recursionCount > 64)
    {
        //Clearly in an infinite recursion situation here!!!
        throw "Error printing tree: infinite recursion!";
    }

    UIDBNode* leftChildNode = convertMe->GetLeftChildNode();
    UIDBNode* rightChildNode = convertMe->GetRightChildNode();

    //Add tree characters.
    for (std::wstring c: startingCharacters)
    {
        wss << c;
    }
    //Convert right child, if any.
    if (rightChildNode == nullptr)
    {
        //No right child.
        wss << UIDBTreeRightChild << L" <leaf>\n";
    }
    else
    {
        //Right child.
        wss << UIDBTreeRightChild << L" " << UIDBNode::ToWString(rightChildNode) << L"\n";
        startingCharacters.push_back(UIDBTreeAncestorNext);
        treeNodeToStringRecursive(wss, startingCharacters, rightChildNode);
        startingCharacters.pop_back();
    }

    //Add tree characters.
    for (std::wstring c: startingCharacters)
    {
        wss << c;
    }
    //Convert left child, if any.
    if (leftChildNode == nullptr)
    {
        //No left child.
        wss << UIDBTreeLeftChild << L" <leaf>\n";
    }
    else
    {
        //Left child.
        wss << UIDBTreeLeftChild << L" " << UIDBNode::ToWString(leftChildNode) << L"\n";
        startingCharacters.push_back(UIDBTreeAncestorNoMore);
        treeNodeToStringRecursive(wss, startingCharacters, leftChildNode);
        startingCharacters.pop_back();
    }

    --recursionCount;
}