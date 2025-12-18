#pragma once

#include <cstddef>
#include <iostream>
#include <array>

template <typename T, std::size_t capacity>
class RingBuffer {
public:
    RingBuffer();
    std::size_t push(const T& element);
    T pop();
    T peek() const;
    std::size_t size() const noexcept;
    bool empty() const noexcept;
    bool full() const noexcept;

    // For testing
    friend std::ostream& operator<<(std::ostream& os, const RingBuffer& buffer) {
        if (buffer.empty()) {
            os << "[]";
            return os;
        }
        
        os << '[';
        for (std::size_t i = buffer.m_head; i != buffer.m_tail; 
            i = (i + 1) % capacity) {
            os << buffer.m_buffer[i] << ", ";
        }
        os << buffer.m_buffer[buffer.m_tail];
        os << ']';
        return os;
    }
private:
    std::size_t m_size;
    std::size_t m_head, m_tail;
    std::array<T, capacity> m_buffer;
};

#include "buffer.tpp"