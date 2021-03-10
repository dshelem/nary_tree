// Исключения
// Автор Д. Шелемех, 2021

#ifndef SDS_EXCEPTIONS_HPP
#define SDS_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

namespace sds {

    // Ошибочный формат типа узла дерева
    class BadNodeTypeFormat: public std::runtime_error
    {
    public:
        BadNodeTypeFormat(std::string const& msg): std::runtime_error(msg) {};
    };

    // Ошибка десериализации (чтения формата файла)
    class DeserialisationException: public std::runtime_error
    {
    public:
        DeserialisationException(std::string const& msg): std::runtime_error(msg) {};
    };


} // namespace sds

#endif