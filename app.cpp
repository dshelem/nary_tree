#include "nary_tree.hpp"
#include "utilities.hpp"
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>

namespace opt = boost::program_options;

int main(int argc, char* argv[])
{
    // Парсинг аргументов командной строки

    opt::options_description desc("All options");
    desc.add_options()
        ("input,i", opt::value<std::string>(), "input file for loading the tree")
        ("output,o", opt::value<std::string>(), "output file for saving the tree")
        ("help,h", "Produce help message")
        ;

    opt::variables_map vm;

    opt::store(opt::parse_command_line(argc, argv, desc), vm);

    opt::notify(vm);

    if(vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    if(!vm.count("help") && !vm.count("input") && !vm.count("output")) {
        std::cout << desc << "\n";
        return 1;
    }

    if(!vm.count("input") && vm.count("output")) {
        std::cout << "Missing required parameter input\n";
        return 1;
    }

    if(vm.count("input") && !vm.count("output")) {
        std::cout << "Missing required parameter output\n";
        return 1;
    }

    // Загрузка дерева, его печать и сохранение

    sds::NaryTree tree = sds::NaryTree();

    sds::loadTreeFromFile(tree, vm["input"].as<std::string>());
    
    if(__linux__ && system("clear")) 
    {
        std::cerr << "Error: unable to clear the screen\n";
        return 1;        
    };

    tree.print();

    sds::saveTreeToFile(tree, vm["output"].as<std::string>());

    return 0;
}