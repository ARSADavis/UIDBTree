#pragma once

#include "Globals.h"

class UIDBNode;
class UIDBNode
{
    public:
        UIDBNode();

        UIDBNode* GetParentNode();
        UIDBNode* GetLeftChildNode();
        UIDBNode* GetRightChildNode();
        UIDBNode* GetNextNode();
        UIDBNode* GetPreviousNode();
        void SetParentNode(UIDBNode* parentNode);
        void SetLeftChildNode(UIDBNode* leftChildNode);
        void SetRightChildNode(UIDBNode* rightChildNode);
        void SetNextNode(UIDBNode* nextNode);
        void SetPreviousNode(UIDBNode* previousNode);

        const ByteVector GetKey();
        const ByteVector GetValue();
        void SetKey(ByteVector key);
        void SetValue(ByteVector value);

        static std::wstring ToWString(UIDBNode* convertMe);

    private:
        std::unique_ptr<UIDBNode> parentNode, leftChildNode, rightChildNode, nextNode, previousNode;
        ByteVector key;
        ByteVector value;
        std::size_t childNodeCount;
};