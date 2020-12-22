#pragma once

#include "../Globals.h"

enum class TreePrintingTypes: unsigned char
{
    VerticalHTML,
    HorizontalHTML
};

class UIDBNode;
class UIDBNode
{
    friend class UIDBTree;

    public:
        UIDBNode();
        ~UIDBNode();

        UIDBNode* GetParentNode();
        UIDBNode* GetLeftChildNode();
        UIDBNode* GetRightChildNode();
        //UIDBNode* GetNextNode();
        //UIDBNode* GetPreviousNode();

        const ByteVector GetKey();
        const ByteVectorVector GetValues();

        static std::wstring ToWString(UIDBNode* convertMe,
            TreePrintingTypes treePrintingType = TreePrintingTypes::VerticalHTML);

    private:
        UIDBNode* parentNode;
        std::unique_ptr<UIDBNode> leftChildNode, rightChildNode; //nextNode, previousNode
        //std::size_t childNodeCount;
        //The equivalent of right.maxDepth - left.maxDepth (if these properties were to be maintained). Note that we
        //only care about the max.
        char subtreeMaxDepthBalance;
        ByteVector key;
        //A vector of values, in order to be able to support duplicates.
        ByteVectorVector values;
};