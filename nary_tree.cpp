#include "nary_tree.hpp"
#include <iostream>

int main()
{
    sds::NaryTree tree(std::make_any<int>(8), std::nullopt, 0);

    sds::Node::PointerType root = tree.getRoot();

    sds::Node::PointerType child1 = tree.addChild(root, std::make_any<std::string>("bar"));
    sds::Node::PointerType child2 = tree.addChild(root, std::make_any<std::string>("baz"));

    sds::Node::PointerType child11 = tree.addChild(child1, std::make_any<double>(2.015));
    tree.addChild(child1, std::make_any<int>(2015));
    tree.addChild(child1, std::make_any<std::string>("2015"));

    tree.addChild(child2, std::make_any<std::string>("foo"));
    sds::Node::PointerType child22 = tree.addChild(child2, std::make_any<double>(6.28318));

    sds::Node::PointerType child111 = tree.addChild(child11, std::make_any<int>(9));
    sds::Node::PointerType child221 = tree.addChild(child22, std::make_any<std::string>("hello"));

    tree.addChild(child111, std::make_any<std::string>("Привет!"));
    tree.addChild(child111, std::make_any<std::string>("Пока!"));
    tree.addChild(child221, std::make_any<double>(3.14159));

    tree.print();

    
    return 0;
}