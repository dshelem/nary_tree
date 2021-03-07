#include "nary_tree.hpp"
#include <iostream>

int main()
{
    sds::Node node = sds::Node(std::make_any<int>(8));
    sds::NaryTree tree(node);

    sds::Node::PointerType root = tree.getRoot();

    sds::Node::PointerType child1 = tree.addChild(root, sds::Node(std::make_any<std::string>("bar")));
    sds::Node::PointerType child2 = tree.addChild(root, sds::Node(std::make_any<std::string>("baz")));

    sds::Node::PointerType child11 = tree.addChild(child1, sds::Node(std::make_any<double>(2.015)));
    tree.addChild(child1, sds::Node(std::make_any<int>(2015)));
    tree.addChild(child1, sds::Node(std::make_any<std::string>("2015")));

    tree.addChild(child2, sds::Node(std::make_any<std::string>("foo")));
    sds::Node::PointerType child22 = tree.addChild(child2, sds::Node(std::make_any<double>(6.28318)));

    tree.addChild(child11, sds::Node(std::make_any<int>(9)));
    tree.addChild(child22, sds::Node(std::make_any<std::string>("hello")));

    tree.print();

    return 0;
}