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

namespace sds {

    class NodeHelperForPrint {
    private:
        // счетчик созданных узлов
        inline static std::size_t counter = 0;

        // данные
        std::size_t number;
        std::optional<std::size_t> parent;
        std::any data;

    public:
        //структоры
        NodeHelperForPrint(std::any const& data, std::optional<std::size_t> const& parent): 
            number(NodeHelperForPrint::counter++), parent(parent), data(data) {}
        NodeHelperForPrint(std::any && data, std::optional<std::size_t> && parent) noexcept:
            number(NodeHelperForPrint::counter++), parent(std::move(parent)), data(std::move(data)) {}
        ~NodeHelperForPrint() = default;

        // IO
        friend std::ostream& operator<<(std::ostream& os, const NodeHelperForPrint& node)
        {
            os << "[ " << node.number << " ] { ";
            if(node.parent) {
                os << *node.parent;
            }
            os << " } " << node.data;
            return os;
        }

        // запросы
        std::size_t getNumber() const noexcept {
            return number;
        }

        using PointerType = std::shared_ptr<NodeHelperForPrint>;
    };

    inline NodeHelperForPrint::PointerType makeNodeHelperForPrintPtr(std::any const& data, 
                std::optional<std::size_t> const& parent = std::nullopt)
    {
        return std::make_shared<NodeHelperForPrint>(data, parent);
    }

    inline NodeHelperForPrint::PointerType makeNodeHelperForPrintPtr(std::any && data, 
                std::optional<std::size_t> && parent = std::nullopt)
    {
        return std::make_shared<NodeHelperForPrint>(std::move(data), std::move(parent));
    }

    class NaryTree
    {
    private:
        Node::PointerType root;

    public:

        // объявления типов
        // указатель на векторы дочерних элементов
        using KidsContainerTypePointer = sds::Node::KidsContainerType*;

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

        // breadth-first algorithm
        std::vector<NodeHelperForPrint::PointerType> prepareData()
        {
            std::vector<NodeHelperForPrint::PointerType> prepared_data;
            std::deque<std::pair<Node::PointerType, std::optional<std::size_t>>> deque;

            std::optional<std::size_t> parent = std::nullopt;
            deque.push_back(std::make_pair(root, parent));

            while(deque.size()) {

                Node::PointerType node_ptr = deque.front().first; // node
                parent = deque.front().second; // parent

                deque.pop_front();

                prepared_data.push_back(makeNodeHelperForPrintPtr(node_ptr->data, parent));
                parent = prepared_data.back()->getNumber();

                for(std::size_t i = 0; i != node_ptr->kids.size(); ++i) {
                    deque.push_back(std::make_pair(node_ptr->kids[i], parent));
                }
            }

            return prepared_data;
        }

        // вывод на экран
        void print()
        {
            std::vector<NodeHelperForPrint::PointerType> print_data = prepareData();

            for(auto node_ptr: print_data) {
                std::cout << *node_ptr << "    ";
            }

            std::cout << "\n";  
        }
    };



} // namespace sds

#endif