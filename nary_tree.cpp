#include "nary_tree.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

sds::NaryTree makeTree()
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

    return tree;
}



void loadTreeFromFile(sds::NaryTree& tree, std::string const& in_file_name)
{
    std::ifstream in_file;
    in_file.open(in_file_name);

    if(!in_file) {
        std::string msg = "Can't open file '" + in_file_name + "' for reading";
        throw std::runtime_error(msg);
    }

    tree.loadTree(in_file);

    in_file.close();
}

void saveTreeToFile(sds::NaryTree& tree, std::string const& out_file_name)
{
    std::ofstream out_file;
    out_file.open(out_file_name, std::ios::trunc);

    if(!out_file) {
        std::string msg = "Can't open file '" + out_file_name + "' for writing";
        throw std::runtime_error(msg);
    }

    tree.saveTree(out_file);
    out_file.close();
}

int main()
{
    const std::string out_file_name = "out_file.txt";
    const std::string in_file_name = "in_file.txt";

    sds::NaryTree tree = sds::NaryTree();

    if(__linux__) system("clear");
    std::cout << "BEFORE LOAD:\n\n";
    tree.print();

    loadTreeFromFile(tree, in_file_name);

    std::cout << "\n\nAFTER LOAD:\n\n";
    tree.print();

    saveTreeToFile(tree, out_file_name);    

    return 0;
}