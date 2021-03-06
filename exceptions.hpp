// Исключения
// Автор Д. Шелемех, 2021

#ifndef SDS_EXCEPTIONS_HPP
#define SDS_EXCEPTIONS_HPP

#include <stdexcept>

namespace sds {

    // Неподдерживаемый формат типа узла дерева

    class BadNodeTypeFormat: public std::runtime_error
    {
    public:
        BadNodeTypeFormat(const char* msg): std::runtime_error(msg) {};
    };

    class DeserialisationException: public std::runtime_error
    {
    public:
        DeserialisationException(const char* msg): std::runtime_error(msg) {};
    };


} // namespace sds


#endif