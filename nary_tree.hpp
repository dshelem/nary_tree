// Модель определения N-ary Tree
// Автор Д. Шелемех, 2021

#ifndef SDS_NARY_TREE_HPP
#define SDS_NARY_TREE_HPP

#include "node.hpp"
#include <deque>
#include <iostream>
#include <cassert>

namespace sds {

    // Класс дерева
    class NaryTree
    {
    private:
        Node::PointerType root;

    public:
        // Структоры
        NaryTree(): root(std::make_shared<Node>(true)) {}
        NaryTree(std::any const& data, std::optional<std::size_t> const& parent, std::size_t level):
            root(std::make_shared<Node>(data, parent, level, true)) {}
        NaryTree(std::any && data, std::optional<std::size_t> && parent, std::size_t level):
            root(std::make_shared<Node>(std::move(data), std::move(parent), level, true)) {}
        NaryTree(Node::PointerType node_ptr): root(node_ptr)
        {
            root->id = 0;
            Node::resetNodeCounter(1);
        }

        ~NaryTree() = default;

        // Аксессоры
        Node::PointerType getRoot() const noexcept {
            return root;
        }
        // Возвращает узел дерева по его id.
        Node::PointerType findNodeById(std::size_t id) {

            // Breadth-first search

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

        // Модификаторы

        // Добавляет потомка для узла дерева.
        // Аргументы:
        // parent - узел
        // data - данные для добавляемого узла
        // Возвращает:
        // Node::PointerType - указатель на добавленный узел.
        Node::PointerType addChild(Node::PointerType& parent, std::any const& data) {
            parent->kids.push_back(sds::makePointer(data, std::make_optional<std::size_t>(parent->id), 
                                    parent->level + 1));
            return parent->kids.back();
        }
        // Добавляет потомка для узла дерева.
        // Аргументы:
        // parent - узел
        // data - данные для добавляемого узла
        // Возвращает:
        // Node::PointerType - указатель на добавленный узел.
        Node::PointerType addChild(Node::PointerType& parent, std::any && data) {
            parent->kids.push_back(sds::makePointer(std::move(data), std::make_optional<std::size_t>(parent->id), 
                                    parent->level + 1));
            return parent->kids.back();
        }
        // Вставляет узел в дерево. Используется при загрузке дерева из файла.
        // Аргументы:
        // Пара: значение типа std::any и id родителя узла
        void insertNode(std::pair<std::any, std::optional<std::size_t>> const& node)
        {
            if(!node.second) {                                                  // root
                root->data = node.first;
                root->type = getNodeTypeFromAny(root->data);
                root->parent = node.second;
            }
            else {                                                              // not root
                Node::PointerType node_to_add_to = findNodeById(*node.second);  // find parent
                if(!node_to_add_to) {
                    std::string msg = "Couldn't find node with ID = " + std::to_string(*node.second);
                    throw std::runtime_error(msg);
                }
                addChild(node_to_add_to, node.first);
            }
        }
        // Подготавливает вектор ссылок на текущие узлы дерева для печати, выгрузки или тестирования.
        // Возвращает:
        // std::vector<Node::PointerType> - вектор
        std::vector<Node::PointerType> getNodesVector()
        {
            // breadth-first algorithm

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
        // Возвращает индекс последнего элемента в векторе, имеющего такой же уровень как и 
        // элемент с индексом start. Используется при печати дерева.
        // Аргументы:
        // vec - вектор ссылок на узлы дерева
        // start - индекс стартового элемента диапазона
        // Возвращает:
        // std::size_t - идекс последнего элемента с таким же уровнем
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
        // Выводит на экран диапазон вектора с индексами [start, end].
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
        // Выводит дерево на экран.
        void print() 
        {
            std::vector<Node::PointerType> print_data(std::move(getNodesVector()));

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
        // Сериализует заголовок формата файла хранения дерева.
        // Аргументы:
        // os - поток для вывода
        void outputHeader(std::ostream& os)
        {
            os << MAGIC_TAG << DELIM << VERSION << "\n";
        }
        // Сериализует дерево.
        // Аргументы:
        // os - поток для вывода
        void saveTree(std::ostream& os)
        {
            std::vector<Node::PointerType> save_data(std::move(getNodesVector()));

            outputHeader(os);
            for(std::size_t i = 0; i != save_data.size(); ++i) {
                os << *save_data[i];
                if(i != (save_data.size() - 1)) {
                    os << EOL;
                }
            }
        }
        // Проверяет правильность заголовка формата хранения дерева.
        // Аргументы:
        // is - поток для ввода
        void checkHeader(std::istream& is)
        {
            char ch;
            std::string tag, version;

            while(is.get(ch))                       // читаем до разделителя
            {
                if(ch != DELIM) {
                    tag.push_back(ch);
                }
                else {
                    break;
                }
            }
            
            if(tag != MAGIC_TAG) {                  // проверяем тэг
                throw std::runtime_error("Wrong input file format");
            }

            while(is.get(ch))
            {
                version.push_back(ch);
            }

            if(std::stoi(version) != VERSION) {     // проверяем версию сериализатора
                throw std::runtime_error("Wrong input file format");
            }
        }
        // Загружает дерево из потока.
        // Аргументы:
        // is - поток для ввода
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