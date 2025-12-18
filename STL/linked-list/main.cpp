#include <iostream>
#include "list.h"

int main() {
    LinkedList<int> l;
    for (int i = 1; i <= 50; ++i) if (i % 5 == 0) l.append(i);

    std::cout << "INITIAL LIST:" << std::endl;
    std::cout << "Expected: [5, 10, 15, 20, 25, 30, 35, 40, 45, 50]" << std::endl;
    std::cout << "Actual:   " << l << '\n' << std::endl;

    l[3] = 1022;

    std::cout << "AFTER SETTING INDEX 3 TO 1022:" << std::endl;
    std::cout << "Expected: [5, 10, 15, 1022, 25, 30, 35, 40, 45, 50]" << std::endl;
    std::cout << "Actual:   " << l << std::endl;

    l.remove(0);
    l.remove(l.size() - 1);

    std::cout << "AFTER REMOVING FIRST AND LAST ELEMENTS:" << std::endl;
    std::cout << "Expected: [10, 15, 1022, 25, 30, 35, 40, 45]" << std::endl;
    std::cout << "Actual:   " << l << std::endl;

    l.insert(2048, 0);
    l.insert(100, 4);

    std::cout << "AFTER INSERTING 2048 AT INDEX 0 AND 100 AT INDEX 4:" << std::endl;
    std::cout << "Expected: [2048, 10, 15, 1022, 100, 25, 30, 35, 40, 45]" << std::endl;
    std::cout << "Actual:   " << l << std::endl;
    std::cout << "Size: " << l.size() << std::endl;

    return 0;
}