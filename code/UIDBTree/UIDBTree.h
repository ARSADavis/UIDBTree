#pragma once

#include "UIDBNode.h"
#include "UIDBTreeResultCode.h"

class UIDBTree
{
    public:
        UIDBTree();
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
        std::unique_ptr<UIDBNode> rootNode;

        static bool compareNodes(UIDBNode* firstNode, UIDBNode* secondNode);

        static void treeNodeToStringRecursive(std::wstringstream& wss, std::vector<std::wstring> startingCharacters,
            UIDBNode* convertMe);
};