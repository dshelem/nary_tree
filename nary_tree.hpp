// https://www.studytonight.com/advanced-data-structures/nary-tree
// https://habr.com/ru/post/273687/
// https://linux.thai.net/~thep/datrie/datrie.html


// Модель определения N-ary Tree
// Автор Д. Шелемех, 2021

#ifndef SDS_NARY_TREE_HPP
#define SDS_NARY_TREE_HPP

#include "node_type.hpp"

namespace sds {

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
        Node::PointerType  addChild(Node::PointerType& parent, Node const& child) {
            parent->kids.push_back(sds::makePointer(child));
            return parent->kids.back();
        }
        Node::PointerType  addChild(Node::PointerType& parent, Node && child) {
            parent->kids.push_back(sds::makePointer(child));
            return parent->kids.back();
        }
        
    };



} // namespace sds

#endif