#include <iostream>
#include "UIDBTree/UIDBTree.h"

int main()
{
    setlocale(LC_ALL, "en_US.UTF-8");
    std::wcout << std::boolalpha;

    std::unique_ptr<UIDBTree> tree(new UIDBTree());

    std::wcout << L"Tree empty: " << tree->IsEmpty() << L"\n";
    std::wcout << L"Tree root node: " << UIDBNode::ToWString(tree->GetRootNode()) << L"\n\n";

    std::pair<UIDBTreeResultCode, UIDBNode*> insertResult;

    insertResult = tree->InsertNodeByKey(StringToBV("yes"), StringToBV("5"));
    std::wcout << L"Inserted { \"yes\": \"5\" } into tree: " << UIDBTreeResultCodeMap.at(insertResult.first) <<
        L" " << UIDBNode::ToWString(insertResult.second) << L"\n\n";

    std::wcout << L"Tree empty: " << tree->IsEmpty() << L"\n";
    std::wcout << L"Tree root node: " << UIDBNode::ToWString(tree->GetRootNode()) << L"\n\n";

    std::wcout << L"Tree contents:\n" << UIDBTree::ToWString(tree.get()) << "\n\n";

    insertResult = tree->InsertNodeByKey(StringToBV("test"), StringToBV("-7"));
    std::wcout << L"Inserted { \"test\": \"-7\" } into tree: " << UIDBTreeResultCodeMap.at(insertResult.first) <<
        L" " << UIDBNode::ToWString(insertResult.second) << L"\n";
    insertResult = tree->InsertNodeByKey(StringToBV("success"), StringToBV("10"));
    std::wcout << L"Inserted { \"success\": \"10\" } into tree: " << UIDBTreeResultCodeMap.at(insertResult.first) <<
        L" " << UIDBNode::ToWString(insertResult.second) << L"\n\n";

    std::wcout << L"Tree contents:\n" << UIDBTree::ToWString(tree.get()) << "\n\n";

    insertResult = tree->InsertNodeByKey(StringToBV("a"), StringToBV("bc"));
    std::wcout << L"Inserted { \"a\": \"bc\" } into tree: " << UIDBTreeResultCodeMap.at(insertResult.first) <<
        L" " << UIDBNode::ToWString(insertResult.second) << L"\n";
    insertResult = tree->InsertNodeByKey(StringToBV("win"), StringToBV("0"));
    std::wcout << L"Inserted { \"win\": \"0\" } into tree: " << UIDBTreeResultCodeMap.at(insertResult.first) <<
        L" " << UIDBNode::ToWString(insertResult.second) << L"\n";
    insertResult = tree->InsertNodeByKey(StringToBV("zoink"), StringToBV("0"));
    std::wcout << L"Inserted { \"zoink\": \"0\" } into tree: " << UIDBTreeResultCodeMap.at(insertResult.first) <<
        L" " << UIDBNode::ToWString(insertResult.second) << L"\n\n";

    std::wcout << L"Tree contents:\n" << UIDBTree::ToWString(tree.get()) << "\n\n";

    std::wcout << L"Needs a rebalance!";


    std::wcout << L"\n\n";
    return 0;
}