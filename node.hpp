// Модуль определения гетерогенного узла дерева
// Автор Д. Шелемех, 2021

#ifndef SDS_NODE_HPP
#define SDS_NODE_HPP

#include "exceptions.hpp"
#include "constants.hpp"
#include <any>
#include <typeinfo>
#include <string>
#include <memory>
#include <vector>
#include <ostream>
#include <istream>
#include <iomanip>
#include <iostream>
#include <optional>

namespace sds {

    // Класс-перечисление фактического типа данных узла.
    // Для расширения добавить перечисления сюда, добавить функцию isNewType(...) и 
    // обновить операторы и функции ввода / вывода
    enum class NodeType {
        Undefined = 0,
        Char = 10, 
        Int = 30,
        Long = 40,
        Double = 50,
        String = 60
    };


    // Функции определения фактического типа данных.
    // Для расширения добавить функцию isNewType(...), обновить перечисление enum class NodeType
    // и операторы / функции ввода / вывода

    inline bool isChar(std::any any) noexcept {
        return any.type() == typeid(char);
    }
    inline bool isInt(std::any any) noexcept {
        return any.type() == typeid(int);
    }
    inline bool isLong(std::any any) noexcept {
        return any.type() == typeid(long);
    }
    inline bool isDouble(std::any any) noexcept {
        return any.type() == typeid(double);
    }
    inline bool isString(std::any any) noexcept {
        return any.type() == typeid(std::string);
    }


    // Для расширения типов добавить обработку нового типа сюда, 
    // добавить функцию isNewType(...) и новый тип в enum class NodeType.
    
    // Используется в выводе в поток узла.

    std::ostream& operator<<(std::ostream& os, const std::any& any)
    {
        if(sds::isChar(any)) {
            if(&os == &std::cout) {
                os << "'" << std::any_cast<char>(any) << "'";
            }
            else {
                os << std::any_cast<char>(any);
            }
        }
        else if(sds::isInt(any))
        {
            os << std::any_cast<int>(any);
        }
        else if(sds::isLong(any))
        {
            os << std::any_cast<long>(any);
        }
        else if(sds::isDouble(any))
        {
            os << std::any_cast<double>(any);
        }
        else if(sds::isString(any))
        {
            std::string string = std::any_cast<std::string>(any);
            if(&os != &std::cout) {
                os << string.size() << sds::DELIM << string;
            }
            else {
                os << std::quoted(string);        
            }
        }
        else {
            std::string msg(__FILE__);
            msg += ": " + std::to_string(__LINE__ - 1) + ": Unsupported format of std::any object";
            throw sds::BadNodeTypeFormat(msg);
        }
        
        return os;
    }

    // Возвращает фактический тип данных узла из аргумента any.
    NodeType getNodeTypeFromAny(std::any const& any) 
    {
        if(sds::isChar(any)) {
            return sds::NodeType::Char;
        }
        else if(sds::isInt(any))
        {
            return sds::NodeType::Int;
        }
        else if(sds::isLong(any))
        {
            return sds::NodeType::Long;
        }
        else if(sds::isDouble(any))
        {
            return sds::NodeType::Double;
        }
        else if(sds::isString(any))
        {
            return sds::NodeType::String;
        }
        else {
            std::string msg(__FILE__);
            msg += ": " + std::to_string(__LINE__ - 1) + ": Unsupported format of std::any object";
            throw sds::BadNodeTypeFormat(msg);
        }
        
        return sds::NodeType::Undefined;
    }

    // Класс узла дерева.
    class Node
    {
    public:
        // Так как дерево будет возвращать узлы после вставки - std::shared_ptr.
        using PointerType = std::shared_ptr<Node>;
        // Контейнер для ссылок на дочерние элементы.
        using KidsContainerType = std::vector<PointerType>;

    private:
        friend class NaryTree;

        inline static std::size_t counter = 0;  // счетчик созданных узлов
        std::size_t id;                         // id узла
        std::optional<std::size_t> parent;      // id родителя
        std::any data;                          // данные
        /*  Выбрал std::any из соображений экономии памяти:
            sizeof(struct for node) = 80
            sizeof(variant) = 40
            sizeof(any) = 16
            и возможности runtime определения фактического типа значения
        */
        NodeType type;                          // тип хранимого значения
        std::size_t level;                      // уровень узла в дереве: 0/1/... (номер строки для вывода на экран)
        KidsContainerType kids;                 // дочерние узлы          

    public:
        // Структоры
        Node(bool reset_counter = false): id(Node::counter++), parent(std::nullopt), 
            data(std::make_any<std::string>("Dummy Node")), 
            type(getNodeTypeFromAny(data)), level(0), kids() 
        {   
            // при создании нового дерева обнуляем счетчик id класса узлов
            if(reset_counter) {
                id = 0;
                counter = 1;
            }
        }
        Node(std::any const& any, std::optional<std::size_t> const& parent, std::size_t level, bool reset_counter = false): 
            id(Node::counter++), parent(parent), data(any), type(getNodeTypeFromAny(data)), level(level), kids()
        {
            // при создании нового дерева обнуляем счетчик id класса узлов
            if(reset_counter) {
                id = 0;
                counter = 1;
            }
        }
        Node(std::any && any, std::optional<std::size_t> && parent, std::size_t level, bool reset_counter = false): 
            id(Node::counter++), parent(parent), data(std::move(any)), type(getNodeTypeFromAny(data)), 
            level(level), kids()
        {
            // при создании нового дерева обнуляем счетчик id класса узлов
            if(reset_counter) {
                id = 0;
                counter = 1;
            }
        }
        Node(Node const& other):
            id(Node::counter++), parent(other.parent), data(other.data), type(other.type), level(other.level), 
            kids(other.kids) {}
        Node(Node && other) noexcept: 
            id(Node::counter++), parent(std::move(other.parent)), data(std::move(other.data)), type(other.type), 
            level(other.level), kids(std::move(other.kids)) {}
        ~Node() = default;

        // Присваивание
        Node& operator=(Node const& ) = delete;
        Node& operator=(Node && ) = delete;

        // Модификаторы
        Node& swap(Node& other) {
            std::swap(id, other.id);
            std::swap(parent, other.parent);
            data.swap(other.data);
            std::swap(type, other.type);
            std::swap(level, other.level);
            kids.swap(other.kids);
            return *this;
        }
        // Обнуляет счетчик id класса узлов (для конструирования нового дерева)
        static void resetNodeCounter(std::size_t new_counter = 0)
        {
            counter = new_counter;
        }

        // Запросы
        bool isEmpty() const noexcept {
            return type == NodeType::Undefined;
        }
        const std::type_info & getType() const noexcept {
            return data.type();
        }
        std::size_t getId() const noexcept {
            return id;
        }
        std::optional<size_t> getParent() const noexcept {
            return parent;
        }
        std::size_t getLevel() const noexcept {
            return level;
        }
        std::any getData() const noexcept {
            return data;
        }

        // IO
        friend std::ostream& operator<<(std::ostream& os, const Node& node)
        {
            // вывод id
            if(&os == &std::cout) {
                os << "[" << node.id << "] ";
            }

            // вывод id родителя
            os << "{";
            if(node.parent) {
                os << *node.parent;
            }
            else {
                os << ROOT_STR;
            }
            os << "} ";

            // вывод данных
            if(&os == &std::cout) {
                os << node.data;
            }
            else {
                os << static_cast<std::underlying_type_t<NodeType>>(node.type) << sds::DELIM << node.data;
            }

            return os;
        }
        // Парсит узел из istream.
        // Возвращает пару из значения типа std::any и id родителя узла.
        static std::pair<std::any, std::optional<std::size_t>> parseNode(std::istream& is)
        {
            char ch;
            int int_value;
            long long_value;
            double double_value;
            std::size_t str_len;
            NodeType node_type;
            std::string string;
            std::optional<std::size_t> opt_parent;
            std::any any_value;

            is.ignore(1); // {

            // parent
            while(is.get(ch))
            {
                if(ch != '}') {
                    string.push_back(ch);
                }
                else {
                    break;
                }
            }

            if(string == ROOT_STR) { // корневой узел
                opt_parent = std::nullopt;
            }
            else {
                opt_parent = std::stoul(string);
            }

            is.ignore(1); // ' ' (space)
            string.clear();

            // node_type
            while(is.get(ch))
            {
                if(ch != DELIM) {
                    string.push_back(ch);
                }
                else {
                    break;
                }
            }

            node_type = static_cast<NodeType>(std::stoi(string));

            string.clear();

            if(node_type == NodeType::Char) {
                is.get(ch);
                any_value = ch;
            }
            else if(node_type == NodeType::Int) {

                while(is.get(ch))
                {
                    string.push_back(ch);
                }

                int_value = std::stoi(string);

                string.clear();

                any_value = int_value;
            }
            else if(node_type == NodeType::Long) {

                while(is.get(ch))
                {
                    string.push_back(ch);
                }

                long_value = std::stol(string);

                string.clear();

                any_value = long_value;
            }
            else if(node_type == NodeType::Double) {

                while(is.get(ch))
                {
                    string.push_back(ch);
                }

                double_value = std::stod(string);

                string.clear();

                any_value = double_value;
            }
            else if(node_type == NodeType::String) {
                // str_len
                while(is.get(ch))
                {
                    if(ch != DELIM) {
                        string.push_back(ch);
                    }
                    else {
                        break;
                    }
                }
                str_len = std::stoul(string);

                string.clear();

                // string itself
                for(std::size_t i = 0; i != str_len; ++i) {
                    is.get(ch);
                    string.push_back(ch);
                }

                any_value = string;

                string.clear();
            }
            else {
                std::string msg(__FILE__);
                msg += ": " + std::to_string(__LINE__ - 1) + ": Unsupported format of std::any object";
                throw sds::DeserialisationException(msg);
            }

            return std::make_pair(any_value, opt_parent);
        }
    };

    // Мейкеры
    inline Node::PointerType 
    makePointer(std::any const& data, std::optional<std::size_t> const& parent, std::size_t level)
    {
        return std::make_shared<Node>(data, parent, level);
    }
    inline Node::PointerType 
    makePointer(std::any && data, std::optional<std::size_t> && parent, std::size_t level)
    {
        return std::make_shared<Node>(std::move(data), std::move(parent), level);
    }
    inline Node::PointerType makePointer(Node const& node) {
        return std::make_unique<Node>(node);
    }
    inline Node::PointerType makePointer(Node && node) {
        return std::make_unique<Node>(std::move(node));
    }

} // namespace sds

#endif