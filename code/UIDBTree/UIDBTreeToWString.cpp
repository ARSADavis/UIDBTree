#include "UIDBTree.h"
#include "UIDBTreeCharacters.h"

std::wstring UIDBTree::ToWString(UIDBTree* convertMe, TreePrintingTypes treePrintingType)
{
    if (convertMe == nullptr)
    {
        if (treePrintingType == TreePrintingTypes::HorizontalHTML)
        {
            return L"<span class=\"leaf\">&lt;N&gt;</span>";
        }
        else
        {
            return L"<span class=\"leaf\">&lt;nullptr&gt;</span>";
        }
    }
    else
    {
        std::wstringstream wss;
        UIDBNode* startingNode = convertMe->GetRootNode();
        if (startingNode == nullptr)
        {
            wss << UIDBNode::ToWString(startingNode, treePrintingType);
            return wss.str();
        }
        switch (treePrintingType)
        {
            case TreePrintingTypes::VerticalHTML:
            {
                std::vector<std::wstring> startingCharacters;
                wss << UIDBNode::ToWString(startingNode, treePrintingType) << L"<br/>";
                UIDBTree::treeNodeToWStringVRecursive(wss, startingCharacters, startingNode);
                break;
            }
            case TreePrintingTypes::HorizontalHTML:
            {
                unsigned char treeMaxDepth = convertMe->GetMaxDepth();
                //Construct a bunch of wstringstreams, two for each level of the tree + one for the deepest leaves.
                std::vector<std::unique_ptr<std::wstringstream>> wssLevels(treeMaxDepth * 2 + 1);
                for (std::unique_ptr<std::wstringstream>& currentWSS: wssLevels)
                {
                    currentWSS.reset(new std::wstringstream());
                }
                //Populate the wstringstreams with the appropriate contents.
                UIDBTree::treeNodeToWStringHRecursive(wssLevels, treeMaxDepth, 0, true, startingNode, false);
                //Output the results.
                for (std::unique_ptr<std::wstringstream>& currentWSS: wssLevels)
                {
                    wss << currentWSS->str() << L"<br/>";
                }
                break;
            }
        }
        return wss.str();
    }
}

void UIDBTree::treeNodeToWStringVRecursive(std::wstringstream& wss, std::vector<std::wstring>& startingCharacters,
    UIDBNode* convertMe)
{
    static char recursionCount = 0;
    ++recursionCount;
    if (recursionCount > 65)
    {
        //Clearly in an infinite recursion situation here!!!
        throw "Error printing tree: infinite recursion!";
    }

    UIDBNode* leftChildNode = convertMe->GetLeftChildNode();
    UIDBNode* rightChildNode = convertMe->GetRightChildNode();

    //Add tree characters.
    wss << L"<span class=\"vtl\">";
    for (std::wstring c: startingCharacters)
    {
        wss << c;
    }
    //Convert right child, if any.
    wss << UIDBVTreeRightChild << L"</span> " <<
        UIDBNode::ToWString(rightChildNode, TreePrintingTypes::VerticalHTML) << L"<br/>";
    if (rightChildNode != nullptr)
    {
        //Right child.
        startingCharacters.push_back(UIDBVTreeAncestorNext);
        treeNodeToWStringVRecursive(wss, startingCharacters, rightChildNode);
        startingCharacters.pop_back();
    }

    //Add tree characters.
    wss << L"<span class=\"vtl\">";
    for (std::wstring c: startingCharacters)
    {
        wss << c;
    }
    //Convert left child, if any.
    wss << UIDBVTreeLeftChild << L"</span> " <<
        UIDBNode::ToWString(leftChildNode, TreePrintingTypes::VerticalHTML) << L"<br/>";
    if (leftChildNode != nullptr)
    {
        //Left child.
        startingCharacters.push_back(UIDBVTreeAncestorNoMore);
        treeNodeToWStringVRecursive(wss, startingCharacters, leftChildNode);
        startingCharacters.pop_back();
    }

    --recursionCount;
}

//Output is something like this:
//                       80
//           ┌───────────┴───────────┐
//           40                      60
//     ┌─────┴─────┐           ┌─────┴─────┐
//     2          <L>         <L>         <L>
//  ┌──┴──┐     ┌──┴──┐
// <L>   <L>   <L>   <L>
//
//or this:
//                                             < 7
//                       ┌───────────────────────┴───────────────────────┐
//                       3                                               9
//           ┌───────────┴───────────┐                       ┌───────────┴───────────┐
//         < 2                       5                       8                       10
//     ┌─────┴─────┐           ┌─────┴─────┐           ┌─────┴─────┐           ┌─────┴─────┐
//     1          <L>          4           6          <L>         <L>         <L>         <L>
//  ┌──┴──┐                 ┌──┴──┐     ┌──┴──┐
// <L>   <L>               <L>   <L>   <L>   <L>
void UIDBTree::treeNodeToWStringHRecursive(std::vector<std::unique_ptr<std::wstringstream>>& wssLevels,
    unsigned char treeMaxDepth, unsigned char currentLevel, bool isFirstOnLevel, UIDBNode* convertMe,
    bool justSpacing)
{
    //Append the current node to the node wstringstream for the current level.

    //Expected to be an odd integer.
    static const float charsPerNodeKey = 5.0;
    static const float charsPerNotch = 1.0;
    static const float charsSideOfNotch = (charsPerNodeKey - charsPerNotch) / 2.0;
    static const float charsPerNodeKeyPlusNotch = charsPerNodeKey + charsPerNotch;
    static const float leftOffset = charsPerNodeKeyPlusNotch / 2.0;
    static const float keyToMidNotch = charsPerNodeKey - charsSideOfNotch;

    float levelsFromBottom = (float)treeMaxDepth - (float)currentLevel;
    size_t nodeKeyIndex = currentLevel * 2;
    size_t treeCharactersIndex = nodeKeyIndex + 1;

    //First, the spacing around the node keys, if any, based upon the levels away from the bottom.
    if (isFirstOnLevel)
    {
        //Spacing to the left of node keys (0 3 9 21 45...).
        *wssLevels[nodeKeyIndex] << std::wstring(
            (intptr_t)(pow(2.0, levelsFromBottom - 1.0) * charsPerNodeKeyPlusNotch - leftOffset), UIDBHTreeEmpty);
    }
    else
    {
        //Spacing in between node keys (1 7 19 43...).
        *wssLevels[nodeKeyIndex] << std::wstring(
            (intptr_t)(pow(2.0, levelsFromBottom) * charsPerNodeKeyPlusNotch - charsPerNodeKey), UIDBHTreeEmpty);
    }

    //Then, the node key itself.
    if (justSpacing)
    {
        *wssLevels[nodeKeyIndex] << std::wstring(charsPerNodeKey, UIDBHTreeEmpty);
    }
    else
    {
        *wssLevels[nodeKeyIndex] << UIDBNode::ToWString(convertMe, TreePrintingTypes::HorizontalHTML);
    }


    //Next, construct the tree characters for the current level (just below the node key).

    //Don't do this, however, if at the lowest level.
    if (levelsFromBottom == 0.0)
    {
        return;
    }

    //Start again with the spacing around the tree notches, if any, based upon the levels away from the bottom.
    if (isFirstOnLevel)
    {
        //Spacing to the left of tree notches (2 5 11 23...).
        *wssLevels[treeCharactersIndex] << std::wstring(
            (intptr_t)(pow(2.0, levelsFromBottom - 2.0) * charsPerNodeKeyPlusNotch - charsPerNotch), UIDBHTreeEmpty);
    }
    else
    {
        //Spacing in between tree notches (5 11 23...).
        *wssLevels[treeCharactersIndex] << std::wstring(
            (intptr_t)(pow(2.0, levelsFromBottom - 1.0) * charsPerNodeKeyPlusNotch - charsPerNotch), UIDBHTreeEmpty);
    }


    //Then, the tree line characters themselves.
    if (convertMe != nullptr)
    {
        *wssLevels[treeCharactersIndex] << L"<span class=\"htl\">";
    }
    //Left notch.
    *wssLevels[treeCharactersIndex] << (convertMe == nullptr ? UIDBHTreeEmpty : UIDBHTreeLeftChild);
    //Lines connecting tree notches (2 5 11 23...).
    *wssLevels[treeCharactersIndex] << std::wstring(
        (intptr_t)(pow(2.0, levelsFromBottom - 2.0) * charsPerNodeKeyPlusNotch - charsPerNotch),
        convertMe == nullptr ? UIDBHTreeEmpty : UIDBHTreeNext);
    //Center parent notch.
    *wssLevels[treeCharactersIndex] << (convertMe == nullptr ? UIDBHTreeEmpty : UIDBHTreeParent);
    //Lines connecting tree notches (2 5 11 23...).
    *wssLevels[treeCharactersIndex] << std::wstring(
        (intptr_t)(pow(2.0, levelsFromBottom - 2.0) * charsPerNodeKeyPlusNotch - charsPerNotch),
        convertMe == nullptr ? UIDBHTreeEmpty : UIDBHTreeNext);
    //Right notch.
    *wssLevels[treeCharactersIndex] << (convertMe == nullptr ? UIDBHTreeEmpty : UIDBHTreeRightChild);
    if (convertMe != nullptr)
    {
        *wssLevels[treeCharactersIndex] << L"</span>";
    }


    //Finally, recurse through all descendants and do the same, depth-first, left child first.
    if (convertMe != nullptr)
    {
        UIDBNode* leftChildNode = convertMe->GetLeftChildNode();
        UIDBNode* rightChildNode = convertMe->GetRightChildNode();
        treeNodeToWStringHRecursive(wssLevels, treeMaxDepth, currentLevel + 1, isFirstOnLevel, leftChildNode, false);
        treeNodeToWStringHRecursive(wssLevels, treeMaxDepth, currentLevel + 1, false, rightChildNode, false);
    }
    else
    {
        //To keep the spacing even, still need to "recurse" but without any data.
        treeNodeToWStringHRecursive(wssLevels, treeMaxDepth, currentLevel + 1, isFirstOnLevel, nullptr, true);
        treeNodeToWStringHRecursive(wssLevels, treeMaxDepth, currentLevel + 1, false, nullptr, true);
    }
}