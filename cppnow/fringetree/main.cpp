import smd.fringetree;

// import <iostream>;

int main(int /*argc*/, char** /*argv*/)
{
    using namespace fringetree;

    using Tree = Tree<int, int>;
    auto t = Tree::branch(
         Tree::branch(Tree::leaf(1), Tree::leaf(2)),
         Tree::leaf(3)
        );

//     auto t1 = prepend(0, t);
//     auto t2 = append(4, t1);

//     //    printer(std::cout, t_);

//     std::cout << "digraph G {\n";
//     printer_ p(std::cout);
//     t->visit(p);
//     t1->visit(p);
//     t2->visit(p);
//     std::cout << "}\n";

//     // auto left = Tree::branch(
//     //     Tree::branch(Tree::leaf(1), Tree::leaf(2)),
//     //     Tree::leaf(3)
//     //     );

//     // auto right = Tree::branch(
//     //     Tree::branch(Tree::leaf(1), Tree::leaf(2)),
//     //     Tree::leaf(3)
//     //     );

//     // auto c = concat(left, right);
//     // printer_ p2(std::cout);
//     // std::cout << "digraph G {\n";
//     // left->visit(p2);
//     // right->visit(p2);
//     // c->visit(p2);
//     // std::cout << "}\n";

}
