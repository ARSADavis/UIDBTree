#pragma once

#include "UIDBNode.h"

class UIDBTree
{
    public:
        //Note: Only relevant properties will be set for each operation; other properties will persist and are expected
        //to be ignored/unused if not directly set by the most recent operation itself.
        class OperationResult
        {
            public:
                //Whether the operation had an error or not. This should always be set. If the result was successful or
                //neutral, e.g. requesting a node when the tree is empty or when the node isn't there, this will be
                //false. This will only be true when there's an actual error and the desired operation could not be
                //completed for some reason, e.g. deleting a node when the tree is empty or when the node isn't there.
                bool HadError = false;

                //The node returned from an operation, if any.
                UIDBNode* Node = nullptr;
                //Whether the node being searched for (e.g. with the exact matching key) was found.
                bool FoundExactNode = false;
                //The tree or subtree depth, if a relevant part of the operation.
                unsigned char Depth = 0;
                //Whether a duplicate key was involved, if a relevant part of the operation.
                bool DuplicateKey = false;

                std::wstring HadErrorToWString()
                {
                    return (HadError ? L"Error!" : L"No error.");
                }

                std::wstring NodeToWString()
                {
                    return UIDBNode::ToWString(Node);
                }

                std::wstring FoundExactNodeToWString()
                {
                    return (FoundExactNode ? L"Found node." : L"Did not find node.");
                }
        };

        OperationResult* Result;

        UIDBTree(bool duplicatesAllowed = true);
        ~UIDBTree();

        bool IsEmpty();
        UIDBNode* GetRootNode();

        OperationResult* GetMaxDepth();

        OperationResult* GetLowestNodeByKey(UIDBNode* topNode);
        OperationResult* GetHighestNodeByKey(UIDBNode* topNode);

        OperationResult* FindNodeOrNearbyByKey(ByteVector key);

        OperationResult* InsertNodeByKey(ByteVector key, ByteVector value);
        OperationResult* DeleteNodeByKey(ByteVector key);

        static std::wstring ToWString(UIDBTree* convertMe, TreePrintingTypes treePrintingType);

    private:
        std::size_t treeNodeCount;
        std::unique_ptr<UIDBNode> rootNode;
        bool duplicatesAllowed;

        void propagateBalanceChange(UIDBNode* parentNode, bool isRightChild, char propagatingChange);

        static char compareKeys(ByteVector firstKey, ByteVector secondKey);

        static void treeNodeToWStringVRecursive(std::wstringstream& wss, std::vector<std::wstring>& startingCharacters,
            UIDBNode* convertMe);
        static void treeNodeToWStringHRecursive(std::vector<std::unique_ptr<std::wstringstream>>& wssLevels,
            unsigned char treeMaxDepth, unsigned char currentLevel, bool isFirstOnLevel, UIDBNode* convertMe,
            bool justSpacing);
};