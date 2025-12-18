#pragma once

#include <cstddef>
#include <cmath>
#include <vector>
#include <iosfwd>
#include <sstream>
#include <format>
#include <functional>
#include <string_view>
#include <initializer_list>
#include <iomanip>
#include <string>

template <typename T, 
          typename compare = std::less<T>, 
          std::size_t branches = 2>
requires (branches >= 2)
class Heap {
public:
    using iterator = std::vector<T>::iterator;
    using const_iterator = std::vector<T>::const_iterator;
    using reverse_iterator = std::vector<T>::reverse_iterator;
    using const_reverse_iterator = std::vector<T>::const_reverse_iterator;

    Heap();
    Heap(std::size_t initial_capacity);

    template <typename InputIt>
    Heap(InputIt first, InputIt last);
    Heap(std::initializer_list<T> list);
    Heap(const std::vector<T>& vec);
    Heap(std::vector<T>&& vec);

    Heap(const Heap& other);
    Heap(Heap&& other);

    Heap& operator=(const Heap& other);
    Heap& operator=(Heap&& other);

    void reserve(std::size_t capacity);

    std::size_t size() const noexcept;
    std::size_t height() const noexcept;
    std::size_t capacity() const noexcept;

    [[nodiscard("You likely meant to use clear()")]]
    bool empty() const noexcept;
    void clear() noexcept;

    void push(const T& element);

    template <typename... Args>
    T& emplace(Args&&... args);

    void insert(std::initializer_list<T> list);
    void insert(const std::vector<T>& vec);

    const T& top() const;
    const T pop();

    iterator begin();
    const_iterator cbegin() const;

    iterator end();
    const_iterator cend() const;
    
    reverse_iterator rbegin();
    const_reverse_iterator crbegin() const;

    reverse_iterator rend();
    const_reverse_iterator crend() const;

    static void sort(std::vector<T>& vec);

    void format_helper(std::vector<std::string>& ss, std::size_t pos, std::size_t height) const {
        std::ostringstream os;
        if (height < ss.size() - 1)
            os << std::left << std::setw(ss[height + 1].size() - ss[height].size());
        os << at(pos);
        if (height == ss.size() - 1) os << ' ';
        ss[height] += os.str();
        std::size_t parent = std::floor((pos - 2) / (float) branches);
        if (pos == (parent + 1) * branches - 1) format_helper(ss, parent, height - 1);
    }


    friend std::ostream& operator<<(std::ostream& os, const Heap& heap) {
        if (heap.empty()) return os;

        const std::size_t height = heap.height();
        std::vector<std::string> ss(height);

        std::size_t pos = static_cast<std::size_t>(
            1 + (std::pow(branches, heap.height() - 1) - 1) / (branches - 1)
        );


        for (; pos <= heap.size(); ++pos) {
            heap.format_helper(ss, pos, height - 1);
        }

        for (const auto& s : ss) os << s << '\n';

        return os;

        // std::size_t row_count = 1;
        // auto it = heap.cbegin();
        // while (it != heap.cend()) {
        //     for (std::size_t col = 0; col < row_count && it != heap.cend(); ++col) {
        //         os << *it << ' ';
        //         ++it;
        //     }
        //     row_count *= branches;
        //     os << "\n";
        // }
        // return os;
    }
private:
    std::vector<T> data_;

    void heapify(std::size_t pos);
    void fix(std::size_t pos);
    void build_heap();
    T& at(std::size_t pos);
    const T& at(std::size_t pos) const;
};

template <typename T>
using MinHeap = Heap<T>;

template <typename T>
using MaxHeap = Heap<T, std::greater<T>>;

namespace std {
    template <typename T, typename compare, std::size_t branches>
    struct formatter<::Heap<T, compare, branches>>
        : formatter<string_view> {
        template <typename FormatContext>
        auto format(const ::Heap<T, compare, branches>& heap, FormatContext& ctx) const {
            ostringstream os;
            os << heap;
            string result = os.str();
            return formatter<string_view>::format(result, ctx);
        }
    };
}

#include "heap.tpp"
