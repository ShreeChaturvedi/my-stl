#include "heap.hpp"

#include <algorithm>
#include <utility>
#include <stdexcept>
#include <cmath>
#include <iterator>

#define HEAP_TEMPLATE                                                                              \
  template <typename T, typename compare, std::size_t branches>                                    \
    requires(branches >= 2)
#define TEMPLATED_HEAP Heap<T, compare, branches>

HEAP_TEMPLATE
TEMPLATED_HEAP::Heap() : data_{} {}

HEAP_TEMPLATE
TEMPLATED_HEAP::Heap(std::size_t initial_capacity) : data_{} {
  data_.reserve(initial_capacity);
}

HEAP_TEMPLATE
template <typename InputIt> TEMPLATED_HEAP::Heap(InputIt first, InputIt last) : data_{} {
  for (auto it = first; it != last; ++it)
    data_.push_back(*it);
  build_heap();
}

HEAP_TEMPLATE
TEMPLATED_HEAP::Heap(std::initializer_list<T> list) : data_{} {
  data_.reserve(list.size());
  for (const auto& v : list)
    data_.push_back(v);
  build_heap();
}

HEAP_TEMPLATE
TEMPLATED_HEAP::Heap(const Vector<T>& vec) : data_{vec} {
  build_heap();
}

HEAP_TEMPLATE
TEMPLATED_HEAP::Heap(Vector<T>&& vec) : data_{std::move(vec)} {
  build_heap();
}

HEAP_TEMPLATE
TEMPLATED_HEAP::Heap(const Heap& other) : data_{other.data_} {}

HEAP_TEMPLATE
TEMPLATED_HEAP::Heap(Heap&& other) : data_{std::move(other.data_)} {
  other.data_.clear();
}

HEAP_TEMPLATE
TEMPLATED_HEAP& TEMPLATED_HEAP::operator=(const TEMPLATED_HEAP& other) {
  data_ = other.data_;
  return *this;
}

HEAP_TEMPLATE
TEMPLATED_HEAP& TEMPLATED_HEAP::operator=(TEMPLATED_HEAP&& other) {
  data_ = std::move(other.data_);
  other.data_.clear();
  return *this;
}

HEAP_TEMPLATE
void TEMPLATED_HEAP::reserve(std::size_t capacity) {
  data_.reserve(capacity);
}

HEAP_TEMPLATE
std::size_t TEMPLATED_HEAP::size() const noexcept {
  return data_.size();
}

HEAP_TEMPLATE
std::size_t TEMPLATED_HEAP::height() const noexcept {
  if (empty())
    return 0;
  return static_cast<std::size_t>(
      std::floor(std::log((size() - 1) * (branches - 1) + 1) / std::log(branches)) + 1);
}

HEAP_TEMPLATE
std::size_t TEMPLATED_HEAP::capacity() const noexcept {
  return data_.capacity();
}

HEAP_TEMPLATE
bool TEMPLATED_HEAP::empty() const noexcept {
  return data_.empty();
}

HEAP_TEMPLATE
void TEMPLATED_HEAP::clear() noexcept {
  data_.clear();
}

HEAP_TEMPLATE
void TEMPLATED_HEAP::push(const T& element) {
  data_.push_back(element);
  fix(data_.size());
}

HEAP_TEMPLATE
template <typename... Args> T& TEMPLATED_HEAP::emplace(Args&&... args) {
  T& result = data_.emplace_back(std::forward<Args>(args)...);
  fix(data_.size());
  return result;
}

HEAP_TEMPLATE
void TEMPLATED_HEAP::insert(std::initializer_list<T> list) {
  for (const T& elem : list)
    push(elem);
}

HEAP_TEMPLATE
void TEMPLATED_HEAP::insert(const Vector<T>& list) {
  for (const auto& elem : list)
    push(elem);
}

HEAP_TEMPLATE
const T& TEMPLATED_HEAP::top() const {
  return data_.front();
}

HEAP_TEMPLATE
const T TEMPLATED_HEAP::pop() {
  if (empty())
    throw std::out_of_range("Cannot pop from empty heap");
  const T result = std::exchange(data_.front(), data_.back());
  data_.pop_back();
  heapify(1);
  return result;
}

HEAP_TEMPLATE
TEMPLATED_HEAP::iterator TEMPLATED_HEAP::begin() {
  return data_.begin();
}

HEAP_TEMPLATE
TEMPLATED_HEAP::const_iterator TEMPLATED_HEAP::cbegin() const {
  return data_.cbegin();
}

HEAP_TEMPLATE
TEMPLATED_HEAP::iterator TEMPLATED_HEAP::end() {
  return data_.end();
}

HEAP_TEMPLATE
TEMPLATED_HEAP::const_iterator TEMPLATED_HEAP::cend() const {
  return data_.cend();
}

HEAP_TEMPLATE
TEMPLATED_HEAP::reverse_iterator TEMPLATED_HEAP::rbegin() {
  return data_.rbegin();
}

HEAP_TEMPLATE
TEMPLATED_HEAP::const_reverse_iterator TEMPLATED_HEAP::crbegin() const {
  return data_.crbegin();
}

HEAP_TEMPLATE
TEMPLATED_HEAP::reverse_iterator TEMPLATED_HEAP::rend() {
  return data_.rend();
}

HEAP_TEMPLATE
TEMPLATED_HEAP::const_reverse_iterator TEMPLATED_HEAP::crend() const {
  return data_.crend();
}

HEAP_TEMPLATE
void TEMPLATED_HEAP::sort(Vector<T>& vec) {
  Heap<T> heap(std::move(vec));
  vec.clear();
  vec.reserve(heap.size());
  while (!heap.empty())
    vec.push_back(heap.pop());
}

HEAP_TEMPLATE
void TEMPLATED_HEAP::heapify(std::size_t pos) {
  std::size_t extremum = pos;
  std::size_t child = (pos - 1) * branches + 2;
  std::size_t end = child + branches;
  for (; child < std::min(end, data_.size() + 1); ++child) {
    if (compare{}(at(child), at(extremum)))
      extremum = child;
  }
  if (extremum == pos)
    return;

  std::swap(at(pos), at(extremum));
  heapify(extremum);
}

HEAP_TEMPLATE
void TEMPLATED_HEAP::fix(std::size_t curr) {
  auto parent_of = [](std::size_t child) -> std::size_t {
    // 1-indexed heap: parent(child) = floor((child - 2) / k) + 1
    if (child <= 1)
      return 0;
    return ((child - 2) / branches) + 1;
  };

  std::size_t parent = parent_of(curr);
  while (curr > 1 && compare{}(at(curr), at(parent))) {
    std::swap(at(curr), at(parent));
    curr = std::exchange(parent, parent_of(parent));
  }
}

HEAP_TEMPLATE
void TEMPLATED_HEAP::build_heap() {
  for (std::size_t pos = data_.size(); pos > 0; --pos)
    heapify(pos);
}

HEAP_TEMPLATE
T& TEMPLATED_HEAP::at(std::size_t pos) {
  return data_[pos - 1];
}

HEAP_TEMPLATE
const T& TEMPLATED_HEAP::at(std::size_t pos) const {
  return data_[pos - 1];
}
