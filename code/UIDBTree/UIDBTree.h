#pragma once

#include "UIDBNode.h"
#include "UIDBTreeResultCode.h"

class UIDBTree
{
    public:
        UIDBTree();
        ~UIDBTree();

        bool IsEmpty();
        observer_ptr<UIDBNode> GetRootNode();

        //observer_ptr<UIDBNode> GetLowestNodeByKey();
        //observer_ptr<UIDBNode> GetHighestNodeByKey();
        std::pair<UIDBTreeResultCode, observer_ptr<UIDBNode>> InsertNodeByKey(ByteVector key, ByteVector value);
        //std::pair<UIDBTreeResultCode, std::vector<observer_ptr<UIDBNode>>> FindNodesByKey();
        //UIDBTreeResultCode DeleteNodeByKey();

    private:
        std::unique_ptr<UIDBNode> rootNode;
};