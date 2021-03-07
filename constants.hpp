// Константы
// Автор Д. Шелемех, 2021

#ifndef SDS_CONSTANTS_HPP
#define SDS_CONSTANTS_HPP

namespace sds {

    // стандартный разделитель между разными данными на одной строке
    const char  DELIM       = ':';
    // конец строки
    const char  EOL         = '\n';
    // формат файла
    const char* MAGIC_TAG   = "sds";
    // версия сериализатора
    const int VERSION       = 1;
    // ширина консоли (в символах)
    const int CON_WIDTH = 136;
    // ширина узла (в символах)
    const int NODE_WIDTH = 18;

} // namespace sds


#endif