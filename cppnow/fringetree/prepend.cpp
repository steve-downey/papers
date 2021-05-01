#include <fringetree/fringetree.h>

#include <iostream>

int main(int /*argc*/, char** /*argv*/)
{
    using namespace fringetree;

    using Tree = Tree<int, int>;
    auto list = Tree::empty();
    auto l1 = prepend(0, list);
    auto l2 = prepend(1, l1);
    auto l3 = prepend(2, l2);
    auto l4 = prepend(3, l3);
    auto l5 = prepend(4, l4);

    std::cout << "digraph G {\n";
    printer_ p2(std::cout);
    list->visit(p2);
    l1->visit(p2);
    l2->visit(p2);
    l3->visit(p2);
    l4->visit(p2);
    l5->visit(p2);
    std::cout << "}\n";
}
