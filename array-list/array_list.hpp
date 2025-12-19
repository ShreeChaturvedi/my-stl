#pragma once

#include <cstddef>
#include <ostream>

template <typename T> class ArrayList {
public:
  /**
   * Default constructor. Assumes initial capacity of 16 and all
   * elements are default initialized.
   */
  ArrayList();

  /**
   * Constructor with an initial capacity. Provided for convinience
   * as the same function can be achieved using the reserve method.
   *
   * \param[in] initial_capacity the initial size of the array
   */
  ArrayList(std::size_t initial_capacity);

  /**
   * Constructor with a length and value. It constructs a new list
   * with length values, each of which are value.
   *
   * \param[in] length the length of the list of elements
   * \param[in] value the value of each one of the length elements
   */
  ArrayList(std::size_t length, T value);

  /**
   * Copy constructor
   *
   * \param[in] other const reference to another ArrayList
   */
  ArrayList(const ArrayList& other);

  /**
   * Move constructor
   *
   * \param[in] other r-value reference to another ArrayList
   */
  ArrayList(ArrayList&& other);

  /**
   * Destructor. Frees the underlying array.
   */
  ~ArrayList();

  /**
   * Copy assignment operator
   *
   * \param[in] other const reference to another ArrayList
   */
  ArrayList& operator=(const ArrayList& other);

  /**
   * Move assignment operator
   *
   * \param[in] other r-value reference to another ArrayList
   */
  ArrayList& operator=(ArrayList&& other) noexcept;

  /**
   * Add another element to the end of the ArrayList
   *
   * \param[in] element the element to be added
   */
  void add(const T& element);

  /**
   * Remove element at specified index
   *
   * \param[in] index the index of the element to be removed
   */
  T remove(std::size_t index);

  /**
   * Insert an element at the given index
   *
   * \param[in] index the index where to insert
   * \param[in] element the element to be inserted
   */
  void insert(std::size_t index, T element);

  /**
   * Subscript operator
   *
   * \param[in] index the index of the reference element
   */
  T& operator[](std::size_t index);

  /**
   * Const subscript operator
   *
   * \param[in] index the index of the reference element
   */
  const T& operator[](std::size_t index) const;

  /**
   * Number of elements currently in the list
   */
  std::size_t size() const noexcept;

  /**
   * Reserve given capacity for the underlying array.
   * This can result in copy of all elements in the array.
   *
   * \param[in] capacity the new capacity to reserve for the array
   */
  void reserve(std::size_t capacity);

  /**
   * Providing stream insertion for convinience
   *
   * \param[in] os the output stream to insert in
   * \param[in] list the ArrayList you want to insert into stream
   */
  friend std::ostream& operator<<(std::ostream& os, const ArrayList& list) {
    os << '[';
    if (list.size_ > 0)
      os << list.data_[0];
    for (std::size_t pos = 1; pos < list.size_; ++pos)
      os << ", " << list.data_[pos];
    return os << ']';
  }

  bool empty() const noexcept;
  std::size_t capacity() const noexcept;

private:
  std::size_t size_;
  std::size_t capacity_;
  T* data_;

  void ensure_capacity();
  T* copy_array();
};

#include "array_list.tpp"
