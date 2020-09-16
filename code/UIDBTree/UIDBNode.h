#pragma once

#include "Globals.h"

class UIDBNode;
class UIDBNode
{
    friend class UIDBTree;

    public:
        UIDBNode();

        UIDBNode* GetParentNode();
        UIDBNode* GetLeftChildNode();
        UIDBNode* GetRightChildNode();
        //UIDBNode* GetNextNode();
        //UIDBNode* GetPreviousNode();

        const ByteVector GetKey();
        const ByteVectorVector GetValues();

        static std::wstring ToWString(UIDBNode* convertMe);

    private:
        std::unique_ptr<UIDBNode> parentNode, leftChildNode, rightChildNode; //nextNode, previousNode
        //std::size_t childNodeCount;
        char subtreeBalance;
        ByteVector key;
        ByteVectorVector values;
};