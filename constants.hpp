// Константы проекта
// Автор Д. Шелемех, 2021

#ifndef SDS_CONSTANTS_HPP
#define SDS_CONSTANTS_HPP

namespace sds {

    // Разделитель между разными данными на одной строке
    const char  DELIM       = ':';
    // Конец строки
    const char  EOL         = '\n';
    // Тэг формата файла
    const char* MAGIC_TAG   = "sds";
    // Версия сериализатора
    const int VERSION       = 1;
    // Идентификатор корня
    const char* ROOT_STR   = "root";
    // Ширина консоли (в символах)
    const int CON_WIDTH = 136;
    // Ширина узла для печати (в символах)
    const int NODE_WIDTH = 14;

} // namespace sds


#endif