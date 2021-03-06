#include "node_type.hpp"
#include <iostream>
#include <variant>

int main()
{
    std::variant<char, short, int, long, double, std::string> variant("123");
    std::any any = std::string("556");
    std::cout << std::boolalpha;
    std::cout << "sizeof(variant) = " << sizeof(variant) << "\n";
    std::cout << "sizeof(any) = " << sizeof(any) << "\n";
    std::cout << "any = " << any << "\n";
    std::cout << "isString(any) = " << sds::isString(any) << "\n";
    any = 1;
    std::cout << "any = " << any << "\n";
    std::cout << "isInt(any) = " << sds::isInt(any) << "\n";
    any = 'a';
    std::cout << "any = " << any << "\n";
    std::cout << "isChar(any) = " << sds::isChar(any) << "\n";
    std::cout << "sizeof(any) = " << sizeof(any) << "\n";
}