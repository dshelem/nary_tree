#include "nary_tree.hpp"
#include "utilities.hpp"
#include <sstream>

int main()
{
    sds::NaryTree tree1 = sds::NaryTree();
    assert(tree1.getNodesVector().size() == 1);
    assert(std::any_cast<std::string>(tree1.getRoot()->getData()) == "Dummy Node");
    std::cout << "[1/4] Passed empty tree test\n";

    sds::NaryTree tree2 = sds::makeSampleTree();
    assert(tree2.getNodesVector().size() == 13);
    std::cout << "[2/4] Passed many nodes tree test\n";

    std::ostringstream os;
    tree2.saveTree(os);
    std::string test_string = "sds:1\n{root} 30:8\n{0} 60:3:bar\n{0} 60:3:baz\n{1} 50:2.015\n";
    test_string += "{1} 30:2015\n{1} 60:4:2015\n{2} 60:3:foo\n{2} 50:6.28318\n{3} 30:9\n{7} 60:5:hello\n";
    test_string += "{8} 60:4:Hey!\n{8} 60:3:Bye\n{9} 50:3.14159";
    assert(os.str() == test_string);
    std::cout << "[3/4] Passed save tree test\n";

    std::istringstream is(os.str());
    sds::NaryTree tree3 = sds::NaryTree();
    tree3.loadTree(is);
    assert(tree3.getNodesVector().size() == 13);
    assert(std::any_cast<std::string>(tree3.findNodeById(2)->getData()) == "baz");
    assert(std::any_cast<double>(tree3.findNodeById(3)->getData()) == 2.015);
    assert(std::any_cast<int>(tree3.findNodeById(8)->getData()) == 9);
    std::cout << "[4/4] Passed load tree test\n";
}