#pragma once

#include "UIDBNode.h"
#include "UIDBTreeResultCode.h"

enum class TreePrintingTypes: unsigned char
{
    VerticalHTML,
    HorizontalHTML
};

class UIDBTree
{
    public:
        UIDBTree(bool duplicatesAllowed = true);
        ~UIDBTree();

        bool IsEmpty();
        UIDBNode* GetRootNode();

        //UIDBNode* GetLowestNodeByKey();
        //UIDBNode* GetHighestNodeByKey();
        std::pair<UIDBTreeResultCode, UIDBNode*> InsertNodeByKey(ByteVector key, ByteVector value);
        //std::pair<UIDBTreeResultCode, std::vector<UIDBNode*>> FindNodesByKey();
        //UIDBTreeResultCode DeleteNodeByKey();

        static std::wstring ToWString(UIDBTree* convertMe, TreePrintingTypes treePrintingType);

    private:
        std::size_t treeNodeCount;
        std::unique_ptr<UIDBNode> rootNode;
        bool duplicatesAllowed;

        void propagateBalanceChange(const std::vector<UIDBNode*>& traversalHistory, UIDBNode* lowestNode,
            char propagatingChange);

        static char compareKeys(ByteVector firstKey, ByteVector secondKey);

        static void treeNodeToWStringVRecursive(std::wstringstream& wss, std::vector<std::wstring>& startingCharacters,
            UIDBNode* convertMe);
        static void treeNodeToWStringHRecursive(std::wstringstream& wss, std::vector<std::wstring>& startingCharacters,
            UIDBNode* convertMe);
};