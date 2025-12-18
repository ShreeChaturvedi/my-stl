#pragma once

#include <cstddef>
#include <initializer_list>
#include <iosfwd>
#include <ostream>

template <typename T>
class Vector {
public:
    using iterator = T*;
    using const_iterator = const T*;

    // Constructors
    Vector() noexcept;
    explicit Vector(std::size_t initial_capacity);
    Vector(std::initializer_list<T> list);
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;

    // Assignment
    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;

    // Destructor
    ~Vector();

    void swap(Vector& other) noexcept;

    // Accessors
    T& operator[](std::size_t pos) noexcept;
    const T& operator[](std::size_t pos) const noexcept;
    T& at(std::size_t pos);
    const T& at(std::size_t pos) const;

    // Setters
    void push_back(const T& element);
    void push_back(T&& element);

    template <typename... Args>
    T& emplace_back(Args&&... args);

    //iterators
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    void clear() noexcept;
    void reserve(std::size_t capacity);
    std::size_t capacity() const noexcept;
    T* data() noexcept;
    const T* data() const noexcept;

    std::size_t size() const noexcept;
    bool empty() const noexcept;

    // For simpler output
    friend std::ostream& operator<<(std::ostream& os, const Vector& vec);
private:
    void ensure_capacity_for_one_more();
    static T* allocate(std::size_t n);
    void deallocate() noexcept;

    std::size_t size_, capacity_;
    T* data_;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& vec) {
    os << '[';
    for (std::size_t i = 0; i < vec.size(); ++i) {
        if (i != 0) os << ", ";
        os << vec[i];
    }
    return os << ']';
}

#include "vector.tpp"
