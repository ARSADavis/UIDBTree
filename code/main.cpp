#include <iostream>
#include "UIDBTree/UIDBTree.h"
#include "HTMLLogger/HTMLLogger.h"
#include "ErrorGlobals.h"

void PrintTreeStatus(HTMLLogger* htmlLogger, std::unique_ptr<UIDBTree>& tree)
{
    htmlLogger->LogTextLine();
    htmlLogger->LogTextLine(L"Tree empty: " + std::to_wstring(tree->IsEmpty()));
    htmlLogger->LogTextLine(L"Tree root node: " + UIDBNode::ToWString(tree->GetRootNode()));
}

void PrintTree(HTMLLogger* htmlLogger, std::unique_ptr<UIDBTree>& tree, TreePrintingTypes treePrintingType)
{
    htmlLogger->LogTextLine();
    htmlLogger->LogTextLine(L"Tree contents:");
    htmlLogger->LogTextLine(L"<pre>" + UIDBTree::ToWString(tree.get(), treePrintingType) + L"</pre>");
}

void InsertData(HTMLLogger* htmlLogger, std::unique_ptr<UIDBTree>& tree, TreeKeyType key, ByteVector value)
{
    std::pair<UIDBTreeResultCode, UIDBNode*> insertResult = tree->InsertNodeByKey(TToBV(key), value);
    htmlLogger->LogTextLine();
    htmlLogger->LogTextLine(L"Inserted { " + UIDBNode::ToWString(insertResult.second) + L" } into tree: " +
        UIDBTreeResultCodeMap.at(insertResult.first));
}

void testTreeInserts(HTMLLogger* htmlLogger)
{
    //Create a UIDBTree.
    std::unique_ptr<UIDBTree> tree(new UIDBTree());
    htmlLogger->LogTextLine(L"New tree");

    PrintTree(htmlLogger, tree, TreePrintingTypes::HorizontalHTML);
    htmlLogger->LogTextLine();

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
        InsertData(htmlLogger, tree, key, StringToBV("data"));
        PrintTree(htmlLogger, tree, TreePrintingTypes::HorizontalHTML);
    }
}

int WrappedMain()
{
    HTMLLogger htmlLogger;
    htmlLogger.OpenLogFile("log/log.html");

    testTreeInserts(&htmlLogger);

    htmlLogger.CloseLogFile();

    return 0;
}

int main()
{
    return MainWithCatch(WrappedMain);
}