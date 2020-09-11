#pragma once

#include "Globals.h"

class UIDBNode;
class UIDBNode
{
    public:
        UIDBNode();

        const ByteVector GetKey();
        const ByteVector GetValue();

        void SetKey(ByteVector key);
        void SetValue(ByteVector value);

    private:
        std::unique_ptr<UIDBNode> parentNode, leftChildNode, rightChildNode, nextNode, previousNode;
        ByteVector key;
        ByteVector value;
        std::size_t childNodeCount;
};

static std::string ObservedNodeToString(observer_ptr<UIDBNode> convertMe)
{
    if (convertMe.get() == nullptr)
    {
        return "<nullptr>";
    }
    else
    {
        std::stringstream resultString;
        UIDBNode* convertMeRaw = convertMe.get();
        const ByteVector key = convertMeRaw->GetKey();
        const ByteVector value = convertMeRaw->GetValue();
        resultString << "{ \"" << std::string(key.begin(), key.end()) << "\", \"" <<
            std::string(value.begin(), value.end()) << "\" }";
        return resultString.str();
    }
}