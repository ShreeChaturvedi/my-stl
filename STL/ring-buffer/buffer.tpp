#include <cstddef>
#include <array>
#include <stdexcept>

template <typename T, std::size_t capacity>
RingBuffer<T, capacity>::RingBuffer() : m_size(0), 
    m_head(0), m_tail(-1), m_buffer{} {}

template <typename T, std::size_t capacity>
std::size_t RingBuffer<T, capacity>::push(const T& element) {
    if (full()) throw std::out_of_range("Cannot push to a full buffer");
    m_tail = (m_tail + 1) % capacity;
    m_buffer[m_tail] = element;
    return ++m_size;
}

template <typename T, std::size_t capacity>
T RingBuffer<T, capacity>::pop() {
    T element = peek();
    m_head = (m_head + 1) % capacity;
    --m_size;
    return element;
}

template <typename T, std::size_t capacity>
T RingBuffer<T, capacity>::peek() const{
    if (empty()) throw std::out_of_range("Cannot pop from an empty buffer");
    return m_buffer[m_head];
}

template <typename T, std::size_t capacity>
std::size_t RingBuffer<T, capacity>::size() const noexcept{
    return m_size;
};

template <typename T, std::size_t capacity>
bool RingBuffer<T, capacity>::empty() const noexcept{
    return m_size == 0;
};

template <typename T, std::size_t capacity>
bool RingBuffer<T, capacity>::full() const noexcept{
    return m_size == capacity;
};