// Модуль типа узла дерева
// Автор Д. Шелемех, 2021

#ifndef SDS_NODE_TYPE_HPP
#define SDS_NODE_TYPE_HPP

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

    // актуальный тип данных узла

    // для расширения добавить перечисления сюда,
    // добавить функцию isNewType(...) и 
    // обновить операторы ввода / вывода

    enum class NodeType {
        Undefined = 0,
        Char = 10, 
        Int = 30,
        Long = 40,
        Double = 50,
        String = 60
    };

    // для расширения добавить функцию isNewType(...), 
    // обновить перечисление enum class NodeTypeб
    // и операторы ввода / вывода

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

    // вывод хранимого значения
    // для расширения типов добавить обработку типа сюда, 
    // добавить функцию isNewType(...) и новый тип в enum class NodeType

    std::ostream& operator<<(std::ostream& os, const std::any& any)
    {
        if(sds::isChar(any)) {
            os << std::any_cast<char>(any);
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
            std::string str(std::to_string(__LINE__));
            str += ": Unsupported format of std::any object";
            throw sds::BadNodeTypeFormat(str.c_str());
        }
        
        return os;
    }

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
            std::string str(std::to_string(__LINE__));
            str += ": Unsupported format of std::any object";
            throw sds::BadNodeTypeFormat(str.c_str());
        }
        
        return sds::NodeType::Undefined;
    }

    // узел дерева

    class Node
    {
    public:
        // объявления типов

        // так как дерево будет возвращать узлы после вставки - расшаренное владение
        using PointerType = std::shared_ptr<Node>;
        // контейнер для ссылок на дочерние элементы
        using KidsContainerType = std::vector<PointerType>;

    private:
        friend class NaryTree;

        // счетчик созданных узлов
        inline static std::size_t counter = 0;

        // ID узла
        std::size_t id;
        // номер родителя
        std::optional<std::size_t> parent;
        // данные
        /*  
            Выбрал std::any из соображений экономии памяти:
            sizeof(struct for node) = 56
            sizeof(variant) = 40
            sizeof(any) = 16 
        */
        std::any data;
        // тип хранимого значения
        NodeType type;
        // уровень (номер строки для вывода на экран)
        std::size_t level;
        // дочерние узлы
        KidsContainerType kids;

    public:
        // структоры
        Node() = delete;
        Node(std::any const& any, std::optional<std::size_t> const& parent, std::size_t level): 
            id(Node::counter++), parent(parent), data(any), type(getNodeTypeFromAny(any)), level(level), kids() {}
        Node(std::any && any, std::optional<std::size_t> && parent, std::size_t level): 
            id(Node::counter++), parent(parent), data(std::move(any)), type(getNodeTypeFromAny(data)), 
            level(level), kids() {}
        Node(Node const& other): 
            id(Node::counter++), parent(other.parent), data(other.data), type(other.type), level(other.level), 
            kids(other.kids) {}
        Node(Node && other) noexcept: 
            id(Node::counter++), parent(std::move(other.parent)), data(std::move(other.data)), type(other.type), 
            level(other.level), kids(std::move(other.kids)) {}
        ~Node() = default;

        // присваивание
        Node& operator=(Node const& ) = delete;
        Node& operator=(Node && ) = delete;

        // модификаторы
        Node& swap(Node& other) {
            std::swap(id, other.id);
            std::swap(parent, other.parent);
            data.swap(other.data);
            std::swap(type, other.type);
            std::swap(level, other.level);
            kids.swap(other.kids);
            return *this;
        }

        // запросы
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

        // IO
        friend std::ostream& operator<<(std::ostream& os, const Node& node)
        {
            // вывод id
            if(&os == &std::cout) {
                os << "[" << node.id << "] ";
            }
            os << "{";

            // вывод родителя
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

        /*
        friend std::istream& operator>>(std::istream& is, Node& node)
        {
            char c, delim;
            short type;
            int i;
            long l;
            double d;
            std::string str;

            is.ignore(1); // [
            is >> node.id; //

            is >> type;
            node.type = static_cast<NodeType>(type);

            switch(node.type)
            {
                case NodeType::Undefined:
                    throw DeserialisationException("Undefined NodeType");
                    break;

                case NodeType::Char:
                    is >> c;
                    node.data = c;
                    break;

                case NodeType::Int:
                    is >> i;
                    node.data = i;
                    break;

                case NodeType::Long:
                    is >> l;
                    node.data = l;
                    break;

                case NodeType::Double:
                    is >> d;
                    node.data = d;
                    break;

                case NodeType::String:
                    int size;
                    is >> size;
                    is >> delim;
                    if(delim != sds::DELIM) {
                        throw DeserialisationException("Wrong input file format");
                    }
                    is.ignore(1);       // первая кавычка
                    for(int i = 0; i != size; ++i) {
                        is.get(c);
                        str.push_back(c);
                    }
                    is.ignore(1);       // вторая кавычка
                    node.data = std::move(str);
                    break;

                default:
                    throw sds::BadNodeTypeFormat("Unsupported format of std::any object");
                    break;
            }

            return is;
        }*/

        
    };

    // мейкеры
    inline Node::PointerType makePointer(Node const& node) {
        return std::make_unique<Node>(node);
    }
    inline Node::PointerType makePointer(Node && node) {
        return std::make_unique<Node>(std::move(node));
    }
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

} // namespace sds

#endif