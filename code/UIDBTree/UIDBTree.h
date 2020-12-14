#pragma once

#include "UIDBNode.h"
#include "UIDBTreeResultCode.h"

class UIDBTree
{
    public:
        UIDBTree(bool duplicatesAllowed = true);
        ~UIDBTree();

        bool IsEmpty();
        UIDBNode* GetRootNode();
        unsigned char GetMaxDepth();

        UIDBNode* GetLowestNodeByKey(UIDBNode* topNode);
        UIDBNode* GetHighestNodeByKey(UIDBNode* topNode);

        std::pair<UIDBTreeResultCode, UIDBNode*> FindNodeByKey(ByteVector key);
        std::pair<UIDBTreeResultCode, std::vector<UIDBNode*>> TraverseToNodeByKey(ByteVector key);

        std::pair<UIDBTreeResultCode, UIDBNode*> InsertNodeByKey(ByteVector key, ByteVector value);
        UIDBTreeResultCode DeleteNodeByKey(ByteVector key);

        static std::wstring ToWString(UIDBTree* convertMe, TreePrintingTypes treePrintingType);

    private:
        std::size_t treeNodeCount;
        std::unique_ptr<UIDBNode> rootNode;
        bool duplicatesAllowed;

        void propagateBalanceChange(const std::vector<UIDBNode*>& traversalHistory, bool isRightChild,
            char propagatingChange);

        static char compareKeys(ByteVector firstKey, ByteVector secondKey);

        static void treeNodeToWStringVRecursive(std::wstringstream& wss, std::vector<std::wstring>& startingCharacters,
            UIDBNode* convertMe);
        static void treeNodeToWStringHRecursive(std::vector<std::unique_ptr<std::wstringstream>>& wssLevels,
            unsigned char treeMaxDepth, unsigned char currentLevel, bool isFirstOnLevel, UIDBNode* convertMe,
            bool justSpacing);
};