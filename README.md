# Гетерогенное Дерево

## Основные файлы проекта
* `app.cpp` исходник консольного приложения, которое загружает дерево, печатает его и сохраняет 
* `constants.hpp` константы, использованные в проекте 
* `exceptions.hpp` кастомные исключения
* `nary_tree.hpp` header-only реализация N-ary дерева
* `nary_tree_test.cpp` тесты реализации N-ary дерева
* `node.hpp` header-only реализация узла гетерогенного дерева
* `utilities.hpp` header-only утилиты проекта

----------

Формат хранения сериализованного дерева (в псевдокоде):
```
<magic_tag><version>
{parent ID} value_type:value

<if string>
{parent ID} value_type:str_len:value
```
См. также файлы `in_file.txt` и `out_file.txt` для наглядного представления формата файла данных.

----------
Ключи компиляции итоговой программы (при них не выдается ни одного замечания от компилятора):
```
/usr/bin/g++ -O0 -g app.cpp -o app -lboost_program_options -std=c++17 -pedantic-errors -Wall -Wextra -Weffc++ -Wsign-conversion
```