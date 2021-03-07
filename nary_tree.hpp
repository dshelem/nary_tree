// https://www.studytonight.com/advanced-data-structures/nary-tree
// https://habr.com/ru/post/273687/
// https://linux.thai.net/~thep/datrie/datrie.html


// Модель определения N-ary Tree
// Автор Д. Шелемех, 2021

#ifndef SDS_NARY_TREE_HPP
#define SDS_NARY_TREE_HPP

#include "node_type.hpp"
#include <iostream>
#include <deque>
#include <optional>
#include <cassert>

namespace sds {

    // хелпер для вывода узлов на печать
    class NodeHelperForPrint {
    private:
        // счетчик созданных узлов
        inline static std::size_t counter = 0;

        // номер узлая
        std::size_t number;
        // данные
        std::any data;
        // номер родителя
        std::optional<std::size_t> parent;
        // уровень (строка для вывода на экран)
        unsigned int level;

    public:
        //структоры
        NodeHelperForPrint(std::any const& data, std::optional<std::size_t> const& parent, unsigned int level): 
            number(NodeHelperForPrint::counter++), data(data), parent(parent), level(level) {}
        NodeHelperForPrint(std::any && data, std::optional<std::size_t> && parent, unsigned int level) noexcept:
            number(NodeHelperForPrint::counter++), data(std::move(data)), parent(std::move(parent)), level(level) {}
        ~NodeHelperForPrint() = default;

        // IO
        friend std::ostream& operator<<(std::ostream& os, const NodeHelperForPrint& node)
        {
            os << "[ " << node.number << " ] { ";
            if(node.parent) {
                os << *node.parent;
            }
            else {
                os << "ROOT";
            }
            os << " } " << node.data;
            // os << " | " << node.level << " |";
            return os;
        }

        // запросы
        std::size_t getNumber() const noexcept {
            return number;
        }
        std::size_t getLevel() const noexcept {
            return level;
        }

        using PointerType = std::shared_ptr<NodeHelperForPrint>;
    };

    // мейкеры для указателей на NodeHelperForPrint
    inline NodeHelperForPrint::PointerType makeNodeHelperForPrintPtr(std::any const& data, 
                std::optional<std::size_t> const& parent, unsigned int level)
    {
        return std::make_shared<NodeHelperForPrint>(data, parent, level);
    }

    inline NodeHelperForPrint::PointerType makeNodeHelperForPrintPtr(std::any && data, 
                std::optional<std::size_t> && parent, unsigned int level)
    {
        return std::make_shared<NodeHelperForPrint>(std::move(data), std::move(parent), level);
    }

    // класс дерева
    class NaryTree
    {
    private:
        Node::PointerType root;

    public:

        // структоры
        NaryTree(): root(std::make_shared<Node>()) {}
        NaryTree(Node const& node): root(std::make_shared<Node>(node)) {}
        NaryTree(Node && node): root(std::make_shared<Node>(std::move(node))) {}
        ~NaryTree() = default;

        // аксессоры
        Node::PointerType getRoot() const noexcept {
            return root;
        }

        // модификаторы
        Node::PointerType addChild(Node::PointerType& parent, Node const& child) {
            parent->kids.push_back(sds::makePointer(child));
            return parent->kids.back();
        }
        Node::PointerType addChild(Node::PointerType& parent, Node && child) {
            parent->kids.push_back(sds::makePointer(child));
            return parent->kids.back();
        }

        using NodeHandlingHelperType = std::tuple<Node::PointerType, std::optional<std::size_t>, unsigned int>;

        // breadth-first algorithm
        std::vector<NodeHelperForPrint::PointerType> prepareData()
        {
            std::vector<NodeHelperForPrint::PointerType> prepared_data;
            std::deque<NodeHandlingHelperType> deque;

            std::optional<std::size_t> parent = std::nullopt; // номер родителя
            unsigned int level = 0; //  уровень для вывода на экран
            deque.push_back(std::make_tuple(root, parent, level));

            while(deque.size()) {

                Node::PointerType node_ptr = std::get<0>(deque.front()); // узел
                parent = std::get<1>(deque.front()); // родитель
                level = std::get<2>(deque.front()); // уровень

                deque.pop_front();

                prepared_data.push_back(makeNodeHelperForPrintPtr(node_ptr->data, parent, level));
                parent = prepared_data.back()->getNumber();

                for(std::size_t i = 0; i != node_ptr->kids.size(); ++i) {
                    deque.push_back(std::make_tuple(node_ptr->kids[i], parent, level + 1));
                }
            }

            return prepared_data;
        }

        std::size_t getLevelFinalIndex(std::vector<NodeHelperForPrint::PointerType> const& vec, std::size_t start) noexcept
        {
            std::size_t level = vec[start]->getLevel(), i = start;

            for(; i < vec.size(); ++i) {
                if(i == vec.size() || vec[i]->getLevel() != level)
                    break;
            }

            return (i - 1);
        }

        void printRange(std::vector<NodeHelperForPrint::PointerType> const& vec, std::size_t start, std::size_t end) 
        {
            assert(start < vec.size());
            assert(start <= end);
            assert(end < vec.size());

            std::size_t count = end - start + 1, width, i;

            if(count == 1) {
                std::cout << std::setw((CON_WIDTH - NODE_WIDTH)/2) << " " << *vec[start];
            }
            else {
                std::cout << *vec[start++];
                width = (CON_WIDTH - NODE_WIDTH * count) / (count - 1);
                for(i = start; i <= end; ++i) {
                    std::cout << std::setw(width) << " " << *vec[i];
                }
            }

            std::cout << "\n\n";
        }

        // вывод на экран
        void print() 
        {
            std::vector<NodeHelperForPrint::PointerType> print_data = std::move(prepareData());

            std::size_t start = 0, end = getLevelFinalIndex(print_data, start);
            assert(end < print_data.size());

            std::cout << std::setw((CON_WIDTH - 14) / 2) << " " << "TREE STRUCTURE\n";
            std::cout << std::setw((CON_WIDTH - 41) / 2) << " " << "[ N ] - Node Number { M } - Parent Number\n\n";

            while(start < print_data.size()) {
                printRange(print_data, start, end);
                start = end + 1;
                if(start < print_data.size()) {
                    end = getLevelFinalIndex(print_data, start);
                }
            }

            std::cout << "\n";
        }
    };



} // namespace sds

#endif