#include "buffer.h"

int main() {
    RingBuffer<int, 8> buffer;

    std::cout << buffer << std::endl;
    std::cout << "Size: " << buffer.size() << std::endl;
    std::cout << "Is empty? " << std::boolalpha << buffer.empty()
                << std::endl;
    std::cout << "Is full? " << std::boolalpha << buffer.full()
                << std::endl;

    for (int i = 1; i <= 7; i += 2) buffer.push(i);
    for (int i = 100; i < 1000; i *= 2) buffer.push(i);
    std::cout << buffer << std::endl;

    std::cout << "Size: " << buffer.size() << std::endl;
    std::cout << "Is empty? " << std::boolalpha << buffer.empty()
                << std::endl;
    std::cout << "Is full? " << std::boolalpha << buffer.full()
                << std::endl;

    // Should throw an exception
    try {
        buffer.push(42);
    } catch (const std::out_of_range& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    std::cout << "Popped: " << buffer.pop() << std::endl;
    std::cout << "Popped: " << buffer.pop() << std::endl;

    std::cout << buffer << std::endl;

    buffer.push(42);
    std::cout << buffer << std::endl;

    std::cout << "Peek: " << buffer.peek() << std::endl;
    std::cout << buffer << std::endl;

    std::cout << "Size: " << buffer.size() << std::endl;
    std::cout << "Is empty? " << std::boolalpha << buffer.empty()
                << std::endl;
    std::cout << "Is full? " << std::boolalpha << buffer.full()
                << std::endl;

    for (int i = 0; i < 3; ++i) {
        std::cout << "Popped: " << buffer.pop() << std::endl;
    }
    std::cout << buffer << std::endl;

    buffer.push(7);
    buffer.push(8);

    std::cout << buffer << std::endl;
    return 0;
}