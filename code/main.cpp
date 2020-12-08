#include <iostream>
#include "UIDBTree/UIDBTree.h"
#include "HTMLLogger/HTMLLogger.h"
#include "ErrorGlobals.h"

void PrintTreeStatus(HTMLLogger* htmlLogger, std::unique_ptr<UIDBTree>& tree)
{
    htmlLogger->LogTextLine(L"Tree empty: " + std::to_wstring(tree->IsEmpty()));
    htmlLogger->LogTextLine(L"Tree root node: " + UIDBNode::ToWString(tree->GetRootNode()));
}

void PrintTree(HTMLLogger* htmlLogger, std::unique_ptr<UIDBTree>& tree, TreePrintingTypes treePrintingType)
{
    htmlLogger->LogWStringInsert(L"<pre>" + UIDBTree::ToWString(tree.get(), treePrintingType) + L"</pre>");
    htmlLogger->LogHR();
}

void InsertData(HTMLLogger* htmlLogger, std::unique_ptr<UIDBTree>& tree, TreeKeyType key, ByteVector value)
{
    std::pair<UIDBTreeResultCode, UIDBNode*> insertResult = tree->InsertNodeByKey(TToBV(key), value);
    htmlLogger->LogTextLine(L"Inserted { " + UIDBNode::ToWString(insertResult.second) +
        L" }: <span class=\"res\">" + UIDBTreeResultCodeMap.at(insertResult.first) + L"</span>");
}

void FindNode(HTMLLogger* htmlLogger, std::unique_ptr<UIDBTree>& tree, TreeKeyType key)
{
    std::pair<UIDBTreeResultCode, UIDBNode*> findResult = tree->FindNodeByKey(TToBV(key));
    htmlLogger->LogTextLine(L"Searched for " + std::to_wstring(key) +
        L": <span class=\"res\">" + UIDBTreeResultCodeMap.at(findResult.first) + L"</span>" +
        (findResult.second == nullptr ? L"" : L" { " + UIDBNode::ToWString(findResult.second) + L" }"));
}

void testTreeInserts(HTMLLogger* htmlLogger)
{
    //Create a UIDBTree.
    std::unique_ptr<UIDBTree> tree(new UIDBTree());
    htmlLogger->LogTextLine(L"New tree");
    PrintTree(htmlLogger, tree, TreePrintingTypes::HorizontalHTML);
    FindNode(htmlLogger, tree, -57);
    FindNode(htmlLogger, tree, 234);

    //These all work.
    //std::vector<TreeKeyType> keys({ 50, 25, 75, 80, 85 });
    //std::vector<TreeKeyType> keys({ 50, 25, 75, 80, 76 });
    //std::vector<TreeKeyType> keys({ 50, 25, 75, 20, 15 });
    //std::vector<TreeKeyType> keys({ 50, 25, 75, 20, 24 });
    //std::vector<TreeKeyType> keys({ 50, 25, 75, 76, 77, 78, 79, 80 });
    std::vector<TreeKeyType> keys({ 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 });
    //std::vector<TreeKeyType> keys({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

    for (TreeKeyType key: keys)
    {
        InsertData(htmlLogger, tree, key, WStringToBV(L"data" + std::to_wstring(key)));
        PrintTree(htmlLogger, tree, TreePrintingTypes::HorizontalHTML);
        for (TreeKeyType key: keys)
        {
            FindNode(htmlLogger, tree, key);
        }
    }

    FindNode(htmlLogger, tree, -57);
    FindNode(htmlLogger, tree, 234);
    htmlLogger->LogNewLine();
    htmlLogger->LogHR();
}

int WrappedMain()
{
    HTMLLogger htmlLogger;
    htmlLogger.OpenLogFile("log/log.html");

    testTreeInserts(&htmlLogger);

    return 0;
}

int main()
{
    return MainWithCatch(WrappedMain);
}