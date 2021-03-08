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
        NaryTree(): root(std::make_shared<Node>()) {}
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
        Node::PointerType findNodeById(std::size_t id) {

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
        void insertNode(std::pair<std::any, std::optional<std::size_t>> const& node)
        {
            if(!node.second) { // root
                root->data = node.first;
                root->type = getNodeTypeFromAny(root->data);
                root->parent = node.second;
            }
            else { // not root
                Node::PointerType node_to_add_to = findNodeById(*node.second); // find parent
                if(!node_to_add_to) {
                    std::string msg = "Couldn't find node with ID = " + std::to_string(*node.second);
                    throw std::runtime_error(msg);
                }
                addChild(node_to_add_to, node.first);
            }
        }

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

        void outputHeader(std::ostream& os)
        {
            os << MAGIC_TAG << DELIM << VERSION << "\n";
        }

        void saveTree(std::ostream& os)
        {
            std::vector<Node::PointerType> save_data(std::move(prepareNodesVector()));

            outputHeader(os);
            for(auto elem: save_data) {
                os << *elem << EOL;
            }
        }

        void checkHeader(std::istream& is)
        {
            char ch;
            std::string tag, version;

            // читаем до разделителя
            while(is.get(ch))
            {
                if(ch != DELIM) {
                    tag.push_back(ch);
                }
                else {
                    break;
                }
            }
            
            if(tag != MAGIC_TAG) {
                throw std::runtime_error("Wrong input file format");
            }

            while(is.get(ch))
            {
                version.push_back(ch);
            }

            if(std::stoi(version) != VERSION) {
                throw std::runtime_error("Wrong input file format");
            }
        }
        void loadTree(std::istream& is)
        {
            std::string line;
            std::getline(is, line, EOL);
            std::istringstream iss(line);
            checkHeader(iss);

            while(std::getline(is, line, EOL))
            {
                std::istringstream iss(line);
                std::pair<std::any, std::optional<std::size_t>> node = Node::parseNode(iss);
                insertNode(node);
            }
        }
    };



} // namespace sds

#endif