#include <iostream>
#include "UIDBTree/UIDBTree.h"
#include "HTMLLogger/HTMLLogger.h"
#include "ErrorGlobals.h"

//TODO: Put these functions in a tester class.

void PrintTreeStatus(HTMLLogger* htmlLogger, std::unique_ptr<UIDBTree>& tree)
{
    htmlLogger->LogTextLine(L"Tree empty: " + std::to_wstring(tree->IsEmpty()));
    htmlLogger->LogTextLine(L"Tree root node: " + UIDBNode::ToWString(tree->GetRootNode()));
}

void PrintTree(HTMLLogger* htmlLogger, std::unique_ptr<UIDBTree>& tree, TreePrintingTypes treePrintingType)
{
    htmlLogger->LogWStringInsert(L"<pre>" + UIDBTree::ToWString(tree.get(), treePrintingType) + L"</pre>");
}

void PrintTreeBounds(HTMLLogger* htmlLogger, std::unique_ptr<UIDBTree>& tree)
{
    htmlLogger->LogTextLine(L"Tree bounds: { " +
        UIDBNode::ToWString(tree->GetLowestNodeByKey(tree->GetRootNode())->Node) + L" } to { " +
        UIDBNode::ToWString(tree->GetHighestNodeByKey(tree->GetRootNode())->Node) + L" }");
}

void InsertData(HTMLLogger* htmlLogger, std::unique_ptr<UIDBTree>& tree, TreeKeyType key, ByteVector value)
{
    UIDBTree::OperationResult* insertResult = tree->InsertNodeByKey(TToBV(key), value);
    htmlLogger->LogTextLine(L"Inserting { " + insertResult->NodeToWString() +
        L" }: <span class=\"res\">" + insertResult->HadErrorToWString() + L"</span>");
}

void FindNode(HTMLLogger* htmlLogger, std::unique_ptr<UIDBTree>& tree, TreeKeyType key)
{
    UIDBTree::OperationResult* findResult = tree->FindNodeOrNearbyByKey(TToBV(key));
    htmlLogger->LogTextLine(L"Searching for " + std::to_wstring(key) +
        L": <span class=\"res\">" + findResult->HadErrorToWString() + L" " + findResult->FoundExactNodeToWString() +
        L"</span>" + (findResult->FoundExactNode ? L" { " + findResult->NodeToWString() + L" }" : L""));
}

void DeleteNode(HTMLLogger* htmlLogger, std::unique_ptr<UIDBTree>& tree, TreeKeyType key)
{
    UIDBTree::OperationResult* deleteResult = tree->DeleteNodeByKey(TToBV(key));
    htmlLogger->LogTextLine(L"Deleting node with key " + std::to_wstring(key) +
        L": <span class=\"res\">" + deleteResult->HadErrorToWString() + L"</span>");
}

void testTreeInserts(HTMLLogger* htmlLogger)
{
    //TODO: Test duplicates!

    //Create a UIDBTree.
    std::unique_ptr<UIDBTree> tree(new UIDBTree());
    htmlLogger->LogTextLine(L"New tree");
    PrintTree(htmlLogger, tree, TreePrintingTypes::HorizontalHTML);
    PrintTreeBounds(htmlLogger, tree);
    htmlLogger->LogNewLine();
    htmlLogger->LogHR();

    FindNode(htmlLogger, tree, 234);
    htmlLogger->LogNewLine();
    htmlLogger->LogHR();

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
        PrintTreeBounds(htmlLogger, tree);
        for (TreeKeyType key: keys)
        {
            FindNode(htmlLogger, tree, key);
        }
        htmlLogger->LogNewLine();
        htmlLogger->LogHR();
    }

    FindNode(htmlLogger, tree, 234);
    htmlLogger->LogNewLine();
    htmlLogger->LogHR();

    //std::vector<TreeKeyType> deleteKeys({ 10, 5, 3, 7, 2, 9, 4, 1, 6, 8 });
    //std::vector<TreeKeyType> deleteKeys({ 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 });
    std::vector<TreeKeyType> deleteKeys({ 1, 2, 4, 6, 5, 3, 7, 10, 8, 9 });
    //std::vector<TreeKeyType> deleteKeys({ 1, 3, 2, 6, 5, 4, 7, 10, 8, 9 });

    for (TreeKeyType key: deleteKeys)
    {
        DeleteNode(htmlLogger, tree, key);
        PrintTree(htmlLogger, tree, TreePrintingTypes::HorizontalHTML);
        PrintTreeBounds(htmlLogger, tree);
        for (TreeKeyType key: keys)
        {
            FindNode(htmlLogger, tree, key);
        }
        htmlLogger->LogNewLine();
        htmlLogger->LogHR();
    }

    DeleteNode(htmlLogger, tree, 9001);
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