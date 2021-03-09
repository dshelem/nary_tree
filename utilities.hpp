// Утилиты проекта
// Автор Д. Шелемех, 2021

#ifndef SDS_UTILITIES_HPP
#define SDS_UTILITIES_HPP

#include "nary_tree.hpp"
#include <fstream>

namespace sds {

    // Коструирует дерево (используется в тестах).
    // Возвращает:
    // sds::NaryTree - дерево
    sds::NaryTree makeSampleTree()
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

        tree.addChild(child111, std::make_any<std::string>("Hey!"));
        tree.addChild(child111, std::make_any<std::string>("Bye"));
        tree.addChild(child221, std::make_any<double>(3.14159));

        return tree;
    }
    // Загружает дерево из файла (обертка для открытия / закрытия файла).
    // Аргументы:
    // tree - дерево
    // in_file_name - имя файла
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
    // Сохраняет дерево в файл (обертка для открытия / закрытия файла).
    // Аргументы:
    // tree - дерево
    // out_file_name - имя файла
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

} // namespace sds

#endif