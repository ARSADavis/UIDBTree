#include "UIDBTree.h"
#include "UIDBTreeCharacters.h"

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
    //Create the node.
    UIDBNode* newNode = new UIDBNode();
    newNode->SetKey(key);
    newNode->SetValue(value);

    //Figure out where to insert it.
    UIDBNode* currentNode = rootNode.get();
    if (currentNode == nullptr)
    {
        rootNode.reset(newNode);
    }
    else
    {
        UIDBNode* lastNode;
        //Compare the nodes and navigate to one of the children. Repeat until an empty leaf node is found. Insert there.
        do
        {
            lastNode = currentNode;
            if (UIDBTree::compareNodes(currentNode, newNode))
            {
                //newNode's key >= currentNode's key; go to the right child.
                currentNode = currentNode->GetRightChildNode();
                if (currentNode == nullptr)
                {
                    //Now that the empty leaf node is found, insert the new node there.
                    lastNode->SetRightChildNode(newNode);
                    break;
                }
            }
            else
            {
                //newNode's key < currentNode's key; go to the left child.
                currentNode = currentNode->GetLeftChildNode();
                if (currentNode == nullptr)
                {
                    //Now that the empty leaf node is found, insert the new node there.
                    lastNode->SetLeftChildNode(newNode);
                    break;
                }
            }
        }
        while (true);
    }
    return { UIDBTreeResultCode::Success, newNode };
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


//Returns true if secondNode's key >= firstNode's key.
bool UIDBTree::compareNodes(UIDBNode* firstNode, UIDBNode* secondNode)
{
    const ByteVector firstKey = firstNode->GetKey();
    const ByteVector secondKey = secondNode->GetKey();
    return secondKey >= firstKey;
}


void UIDBTree::treeNodeToStringRecursive(std::wstringstream& wss, std::vector<std::wstring> startingCharacters,
    UIDBNode* convertMe)
{
    UIDBNode* leftChildNode = convertMe->GetLeftChildNode();
    UIDBNode* rightChildNode = convertMe->GetRightChildNode();

    //Convert right child, if any.
    for (std::wstring c: startingCharacters)
    {
        wss << c;
    }
    if (rightChildNode == nullptr)
    {
        //No right child.
        wss << UIDBTreeRightChild << L" <leaf>\n";
    }
    else
    {
        //Right child.
        wss << UIDBTreeRightChildMore << L" " << UIDBNode::ToWString(rightChildNode) << L"\n";
        startingCharacters.push_back(leftChildNode == nullptr ? UIDBTreeAncestorNoMore : UIDBTreeAncestorNext);
        treeNodeToStringRecursive(wss, startingCharacters, rightChildNode);
        startingCharacters.pop_back();
    }

    //Convert left child, if any.
    for (std::wstring c: startingCharacters)
    {
        wss << c;
    }
    if (leftChildNode == nullptr)
    {
        //No left child.
        wss << UIDBTreeLeftChild << L" <leaf>\n";
    }
    else
    {
        //Left child.
        wss << UIDBTreeLeftChildMore << L" " << UIDBNode::ToWString(leftChildNode) << L"\n";
        startingCharacters.push_back(UIDBTreeAncestorNoMore);
        treeNodeToStringRecursive(wss, startingCharacters, leftChildNode);
        startingCharacters.pop_back();
    }
}