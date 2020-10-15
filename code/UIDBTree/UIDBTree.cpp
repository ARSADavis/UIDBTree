#include "UIDBTree.h"
#include "UIDBTreeCharacters.h"

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
        UIDBNode* lastNode;
        char comparisonValue;
        do
        {
            //Keep track of the traversal history down to the inserted node, for later rebalancing, if needed.
            lastNode = currentNode;
            //Compare the current node's key with the given key, to decide what to do next.
            comparisonValue = UIDBTree::compareKeys(currentNode->key, key);
            if (comparisonValue > 0)
            {
                //New key > current key.

                //Balance of the ancestral subtree tips to the right one, whether the new node is inserted here or not.
                ++lastNode->subtreeBalance;
                //Navigate to the right child.
                currentNode = currentNode->rightChildNode.get();
                if (currentNode == nullptr)
                {
                    //Now that the empty leaf node is found, insert the new node there.
                    currentNode = new UIDBNode();
                    currentNode->key = key;
                    currentNode->values = ByteVectorVector({ value });
                    lastNode->rightChildNode.reset(currentNode);
                    ++treeNodeCount;
                    break;
                }
            }
            else if (comparisonValue < 0)
            {
                //New key < current key.

                //Balance of the ancestral subtree tips to the left one, whether the new node is inserted here or not.
                --lastNode->subtreeBalance;
                //Navigate to the left child.
                currentNode = currentNode->leftChildNode.get();
                if (currentNode == nullptr)
                {
                    //Now that the empty leaf node is found, insert the new node there.
                    currentNode = new UIDBNode();
                    currentNode->key = key;
                    currentNode->values = ByteVectorVector({ value });
                    lastNode->leftChildNode.reset(currentNode);
                    ++treeNodeCount;
                    break;
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
        //If a new node was inserted as a child node in particular, then there may need to be a rebalance.
        rebalanceFrom(lastNode);
    }

    return { UIDBTreeResultCode::Success, currentNode };
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


//Returns the sign of the comparison of the two keys: second > first: 1, second < first: -1, second == first: 0.
char UIDBTree::compareKeys(ByteVector firstKey, ByteVector secondKey)
{
    return (secondKey > firstKey ? 1 : (secondKey < firstKey ? -1 : 0));
}


void UIDBTree::rebalanceFrom(UIDBNode* startNode)
{
    UIDBNode* currentNode = startNode;
    //Check for the need to rebalance each of the inserted node's ancestors, starting with the parent of the
    //inserted node and working up to the root node.
    do
    {
        if (currentNode->subtreeBalance > 1)
        {
            //Right subtree is too heavy compared to left subtree.


            //TODO
        }
        else if (currentNode->subtreeBalance < -1)
        {
            //Left subtree is too heavy compared to right subtree.


            //TODO
        }
        currentNode = currentNode->parentNode.get();
    }
    while (currentNode != nullptr);
}


void UIDBTree::treeNodeToStringRecursive(std::wstringstream& wss, std::vector<std::wstring> startingCharacters,
    UIDBNode* convertMe)
{
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
}