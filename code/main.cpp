#include <iostream>
#include "UIDBTree/UIDBTree.h"

int main()
{
    std::cout << std::boolalpha;

    std::unique_ptr<UIDBTree> tree(new UIDBTree());

    std::cout << "Tree empty: " << tree->IsEmpty() << "\n";
    std::cout << "Tree root node: " << ObservedNodeToString(tree->GetRootNode()) << "\n\n";

    std::pair<UIDBTreeResultCode, observer_ptr<UIDBNode>> insertResult =
        tree->InsertNodeByKey({ 'y', 'e', 's' }, { '5' });
    std::cout << "Inserted { \"yes\": \"5\" } into tree: " << UIDBTreeResultCodeMap.at(insertResult.first) <<
        " " << ObservedNodeToString(insertResult.second) << "\n\n";

    std::cout << "Tree empty: " << tree->IsEmpty() << "\n";
    std::cout << "Tree root node: " << ObservedNodeToString(tree->GetRootNode()) << "\n\n";

    std::cout << "\n\n";
    return 0;
}