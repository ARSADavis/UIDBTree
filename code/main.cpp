#include <iostream>
#include "UIDBTree/UIDBTree.h"
#include "HTMLLogger/HTMLLogger.h"

void PrintTreeStatus(HTMLLogger* htmlLogger, std::unique_ptr<UIDBTree>& tree)
{
    htmlLogger->LogTextLine();
    htmlLogger->LogTextLine(L"Tree empty: " + std::to_wstring(tree->IsEmpty()));
    htmlLogger->LogTextLine(L"Tree root node: " + UIDBNode::ToWString(tree->GetRootNode()));
}

void PrintTree(HTMLLogger* htmlLogger, std::unique_ptr<UIDBTree>& tree)
{
    htmlLogger->LogTextLine();
    htmlLogger->LogTextLine(L"Tree contents:");
    htmlLogger->LogTextLine(L"<pre>" + UIDBTree::ToWString(tree.get()) + L"</pre>");
}

void InsertData(HTMLLogger* htmlLogger, std::unique_ptr<UIDBTree>& tree, TreeKeyType key, ByteVector value)
{
    std::pair<UIDBTreeResultCode, UIDBNode*> insertResult = tree->InsertNodeByKey(TToBV(key), value);
    htmlLogger->LogTextLine();
    htmlLogger->LogTextLine(L"Inserted { " + std::to_wstring(key) + L": " + BVToWString(value) + L" } into tree: " +
        UIDBTreeResultCodeMap.at(insertResult.first) + L" " + UIDBNode::ToWString(insertResult.second));
}

void testTree(HTMLLogger* htmlLogger)
{
    //Create a UIDBTree.
    std::unique_ptr<UIDBTree> tree(new UIDBTree());
    htmlLogger->LogTextLine(L"Created tree");

    PrintTreeStatus(htmlLogger, tree);
    PrintTree(htmlLogger, tree);
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
        PrintTreeStatus(htmlLogger, tree);
        PrintTree(htmlLogger, tree);
    }
}

int main()
{
    //UTF-8 wide character printing.
    setlocale(LC_ALL, "en_US.UTF-8");
    std::wcout << std::boolalpha;

    HTMLLogger htmlLogger;
    htmlLogger.OpenLogFile("log/log.html");

    testTree(&htmlLogger);

    htmlLogger.CloseLogFile();

    std::wcout << L"\n\n";
    return 0;
}