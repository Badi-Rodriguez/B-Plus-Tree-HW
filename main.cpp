#include "BPlusTree.h"

int main() {
    BPlusTree tree;
    tree.insert(10, 100);
    tree.insert(20, 200);
    tree.insert(30, 300);
    tree.insert(40, 400);

    std::cout << "Search for 20: " << tree.search(20) << std::endl;
    std::cout << "Search for 25: " << tree.search(25) << std::endl;

    tree.remove(20);

    std::cout << "Search for 20 after deletion: " << tree.search(20) << std::endl;

    return 0;
}
