#include <cstddef>
#include <concepts>
#include <random>
#include <iostream>
#include <utility>

#include <algorithm>

#include "heap.hpp"
#include "vector/vector.hpp"

int main(/*int argc, char* argv[]*/) {
    Vector<int> vec;

    // std::mt19937 gen(std::random_device{}());
    // std::uniform_int_distribution<int> dist(1, 1000);

    // std::generate_n(std::back_inserter(vec), 45, 
    //                 [&dist, &gen]() { return dist(gen); });

    for (int i = 1; i < 14; ++i) vec.push_back(i);

    Heap<int, std::less<int>, 3> heap(vec);
    std::cout << heap.size() << std::endl;

    std::cout << heap << std::endl;

    return 0;

    // if (argc < 2) {
    //     std::cout << "Usage: " << argv[0] << " <std|custom> [elements]\n";
    //     return -1;
    // }

    // std::vector<int> vec;
    // const std::size_t elements = (argc >=3) ? std::stoull(argv[2]) : 1'000'000;
    // vec.reserve(elements);

    // std::mt19937 gen(std::random_device{}());
    // std::uniform_int_distribution<int> dist(1, 100);

    // std::generate_n(std::back_inserter(vec), elements, 
    //                 [&dist, &gen]() { return dist(gen); });

    // std::cout << std::boolalpha;
    // std::cout << "\nSorted: " << std::is_sorted(vec.begin(), vec.end()) << '\n';

    // std::string sort_arg = argv[1];
    // if (sort_arg == "std") std::sort(vec.begin(), vec.end());
    // else if (sort_arg == "custom") Heap<int>::sort(vec);
    // else return -1;

    // std::cout << "Sorted: " << std::is_sorted(vec.begin(), vec.end()) << '\n';
    // std::cout << std::endl;

    return 0;
}
