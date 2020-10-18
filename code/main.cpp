#include <iostream>
#include "UIDBTree/UIDBTree.h"

void PrintTreeStatus(std::unique_ptr<UIDBTree>& tree)
{
    std::wcout << L"\nTree empty: " << tree->IsEmpty() << L"\n";
    std::wcout << L"Tree root node: " << UIDBNode::ToWString(tree->GetRootNode());
}

void PrintTree(std::unique_ptr<UIDBTree>& tree)
{
    std::wcout << L"\nTree contents:\n" << UIDBTree::ToWString(tree.get());
}

void InsertData(std::unique_ptr<UIDBTree>& tree, TreeKeyType key, ByteVector value)
{
    std::pair<UIDBTreeResultCode, UIDBNode*> insertResult = tree->InsertNodeByKey(TToBV(key), value);
    std::wcout << L"\nInserted { " << key << L": " << BVToWString(value) << L" } into tree: " <<
        UIDBTreeResultCodeMap.at(insertResult.first) << L" " << UIDBNode::ToWString(insertResult.second);
}

int main()
{
    //UTF-8 wide character printing.
    setlocale(LC_ALL, "en_US.UTF-8");
    std::wcout << std::boolalpha;

    //Create a UIDBTree.
    std::unique_ptr<UIDBTree> tree(new UIDBTree());
    std::wcout << L"Created tree";

    PrintTreeStatus(tree);
    PrintTree(tree);
    std::wcout << L"\n";

    //These all work.
    //std::vector<TreeKeyType> keys({ 50, 25, 75, 80, 85 });
    //std::vector<TreeKeyType> keys({ 50, 25, 75, 80, 76 });
    //std::vector<TreeKeyType> keys({ 50, 25, 75, 20, 15 });
    //std::vector<TreeKeyType> keys({ 50, 25, 75, 20, 24 });
    //std::vector<TreeKeyType> keys({ 50, 25, 75, 76, 77, 78, 79, 80 });
    //std::vector<TreeKeyType> keys({ 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 });
    std::vector<TreeKeyType> keys({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

    for (TreeKeyType key: keys)
    {
        InsertData(tree, key, StringToBV("data"));
        PrintTreeStatus(tree);
        PrintTree(tree);
    }

    std::wcout << L"\n\nFinished.";

    std::wcout << L"\n\n";
    return 0;
}