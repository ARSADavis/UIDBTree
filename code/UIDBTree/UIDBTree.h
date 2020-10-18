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

        //UIDBNode* GetLowestNodeByKey();
        //UIDBNode* GetHighestNodeByKey();
        std::pair<UIDBTreeResultCode, UIDBNode*> InsertNodeByKey(ByteVector key, ByteVector value);
        //std::pair<UIDBTreeResultCode, std::vector<UIDBNode*>> FindNodesByKey();
        //UIDBTreeResultCode DeleteNodeByKey();

        static std::wstring ToWString(UIDBTree* convertMe);

    private:
        std::size_t treeNodeCount;
        std::unique_ptr<UIDBNode> rootNode;
        bool duplicatesAllowed;

        void propagateBalanceChange(const std::vector<UIDBNode*>& traversalHistory, UIDBNode* lowestNode,
            char propagatingChange);

        static char compareKeys(ByteVector firstKey, ByteVector secondKey);

        static void treeNodeToStringRecursive(std::wstringstream& wss, std::vector<std::wstring>& startingCharacters,
            UIDBNode* convertMe);
};