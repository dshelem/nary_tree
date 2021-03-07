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
#include <cassert>

namespace sds {

    // класс дерева
    class NaryTree
    {
    private:
        Node::PointerType root;

    public:

        // структоры
        NaryTree(std::any const& data, std::optional<std::size_t> const& parent, std::size_t level): 
            root(std::make_shared<Node>(data, parent, level)) {}
        NaryTree(std::any && data, std::optional<std::size_t> && parent, std::size_t level): 
            root(std::make_shared<Node>(std::move(data), std::move(parent), level)) {}
        NaryTree(Node::PointerType node_ptr): root(node_ptr) {}
        ~NaryTree() = default;

        // аксессоры
        Node::PointerType getRoot() const noexcept {
            return root;
        }
        // breadth-first search
        Node::PointerType getNodeById(std::size_t id) {

            std::deque<Node::PointerType> deque;
            deque.push_back(root);

            while(deque.size()) {

                Node::PointerType node_ptr = deque.front(); deque.pop_front();

                if(node_ptr->id == id) {
                    return node_ptr;
                }

                for(std::size_t i = 0; i != node_ptr->kids.size(); ++i) {
                    deque.push_back(node_ptr->kids[i]);
                }
            }

            return nullptr;
        }

        // модификаторы
        Node::PointerType addChild(Node::PointerType& parent, std::any const& data) {
            parent->kids.push_back(sds::makePointer(data, std::make_optional<std::size_t>(parent->id), parent->level + 1));
            return parent->kids.back();
        }
        Node::PointerType addChild(Node::PointerType& parent, std::any && data) {
            parent->kids.push_back(sds::makePointer(std::move(data), std::make_optional<std::size_t>(parent->id), parent->level + 1));
            return parent->kids.back();
        }

        using NodeHandlingHelperType = std::tuple<Node::PointerType, std::optional<std::size_t>, std::size_t>;

        // breadth-first algorithm
        std::vector<Node::PointerType> prepareNodesVector()
        {
            std::vector<Node::PointerType> vec;
            std::deque<Node::PointerType> deque;

            deque.push_back(root);

            while(deque.size()) {

                Node::PointerType node_ptr = deque.front(); deque.pop_front();

                vec.push_back(node_ptr);

                for(std::size_t i = 0; i != node_ptr->kids.size(); ++i) {
                    deque.push_back(node_ptr->kids[i]);
                }
            }

            return vec;
        }

        std::size_t getFinalIndexOfTheSameLevel(std::vector<Node::PointerType> const& vec, std::size_t start) noexcept
        {
            assert(start < vec.size());

            std::size_t level = vec[start]->level, i = start;

            for(; i < vec.size(); ++i) {
                if(i == vec.size() || vec[i]->level != level)
                    break;
            }

            return (i - 1);
        }

        void printRange(std::vector<Node::PointerType> const& vec, std::size_t start, std::size_t end) 
        {
            assert(start < vec.size());
            assert(end < vec.size());
            assert(start <= end);

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
            std::vector<Node::PointerType> print_data(std::move(prepareNodesVector()));

            std::size_t start = 0, end = getFinalIndexOfTheSameLevel(print_data, start);

            std::cout << std::setw((CON_WIDTH - 14) / 2) << " " << "TREE STRUCTURE\n";
            std::cout << std::setw((CON_WIDTH - 33) / 2) << " " << "[N] - Node ID {P} - Parent ID\n\n";

            while(start < print_data.size()) {
                printRange(print_data, start, end);
                start = end + 1;
                if(start < print_data.size()) {
                    end = getFinalIndexOfTheSameLevel(print_data, start);
                }
            }

            std::cout << "\n";
        }
    };



} // namespace sds

#endif