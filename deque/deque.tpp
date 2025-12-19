template <typename T>
Deque<T>::Deque() noexcept : data_(nullptr), size_(0), capacity_(0), head_(0) {}

template <typename T> Deque<T>::Deque(const Deque& other) : Deque() {
  if (other.size_ == 0)
    return;
  grow(other.size_);
  for (size_type i = 0; i < other.size_; ++i)
    push_back(other[i]);
}

template <typename T>
Deque<T>::Deque(Deque&& other) noexcept
    : alloc_(), data_(std::exchange(other.data_, nullptr)), size_(std::exchange(other.size_, 0)),
      capacity_(std::exchange(other.capacity_, 0)), head_(std::exchange(other.head_, 0)) {}

template <typename T> Deque<T>::~Deque() {
  destroy_all();
  deallocate();
}

template <typename T> Deque<T>& Deque<T>::operator=(const Deque& other) {
  if (this == &other)
    return *this;
  Deque tmp(other);
  *this = std::move(tmp);
  return *this;
}

template <typename T> Deque<T>& Deque<T>::operator=(Deque&& other) noexcept {
  if (this == &other)
    return *this;
  destroy_all();
  deallocate();
  data_ = std::exchange(other.data_, nullptr);
  size_ = std::exchange(other.size_, 0);
  capacity_ = std::exchange(other.capacity_, 0);
  head_ = std::exchange(other.head_, 0);
  return *this;
}

template <typename T> bool Deque<T>::empty() const noexcept {
  return size_ == 0;
}

template <typename T> typename Deque<T>::size_type Deque<T>::size() const noexcept {
  return size_;
}

template <typename T>
typename Deque<T>::size_type Deque<T>::phys_index(size_type i) const noexcept {
  return capacity_ == 0 ? 0 : (head_ + i) % capacity_;
}

template <typename T> T& Deque<T>::operator[](size_type i) noexcept {
  return data_[phys_index(i)];
}

template <typename T> const T& Deque<T>::operator[](size_type i) const noexcept {
  return data_[phys_index(i)];
}

template <typename T> T& Deque<T>::at(size_type i) {
  if (i >= size_)
    throw std::out_of_range("Deque::at out of range");
  return (*this)[i];
}

template <typename T> const T& Deque<T>::at(size_type i) const {
  if (i >= size_)
    throw std::out_of_range("Deque::at out of range");
  return (*this)[i];
}

template <typename T> T& Deque<T>::front() {
  if (empty())
    throw std::out_of_range("Deque::front on empty");
  return (*this)[0];
}

template <typename T> const T& Deque<T>::front() const {
  if (empty())
    throw std::out_of_range("Deque::front on empty");
  return (*this)[0];
}

template <typename T> T& Deque<T>::back() {
  if (empty())
    throw std::out_of_range("Deque::back on empty");
  return (*this)[size_ - 1];
}

template <typename T> const T& Deque<T>::back() const {
  if (empty())
    throw std::out_of_range("Deque::back on empty");
  return (*this)[size_ - 1];
}

template <typename T> void Deque<T>::clear() noexcept {
  destroy_all();
  size_ = 0;
  head_ = 0;
}

template <typename T> void Deque<T>::push_back(const T& value) {
  ensure_capacity_for_one_more();
  const size_type pos = phys_index(size_);
  std::construct_at(data_ + pos, value);
  ++size_;
}

template <typename T> void Deque<T>::push_back(T&& value) {
  ensure_capacity_for_one_more();
  const size_type pos = phys_index(size_);
  std::construct_at(data_ + pos, std::move(value));
  ++size_;
}

template <typename T> void Deque<T>::push_front(const T& value) {
  ensure_capacity_for_one_more();
  head_ = (head_ + capacity_ - 1) % capacity_;
  std::construct_at(data_ + head_, value);
  ++size_;
}

template <typename T> void Deque<T>::push_front(T&& value) {
  ensure_capacity_for_one_more();
  head_ = (head_ + capacity_ - 1) % capacity_;
  std::construct_at(data_ + head_, std::move(value));
  ++size_;
}

template <typename T> void Deque<T>::pop_back() {
  if (empty())
    throw std::out_of_range("Deque::pop_back on empty");
  const size_type pos = phys_index(size_ - 1);
  std::destroy_at(data_ + pos);
  --size_;
  if (size_ == 0)
    head_ = 0;
}

template <typename T> void Deque<T>::pop_front() {
  if (empty())
    throw std::out_of_range("Deque::pop_front on empty");
  std::destroy_at(data_ + head_);
  head_ = (head_ + 1) % capacity_;
  --size_;
  if (size_ == 0)
    head_ = 0;
}

template <typename T> typename Deque<T>::iterator Deque<T>::begin() noexcept {
  return iterator(this, 0);
}

template <typename T> typename Deque<T>::iterator Deque<T>::end() noexcept {
  return iterator(this, size_);
}

template <typename T> typename Deque<T>::const_iterator Deque<T>::begin() const noexcept {
  return const_iterator(this, 0);
}

template <typename T> typename Deque<T>::const_iterator Deque<T>::end() const noexcept {
  return const_iterator(this, size_);
}

template <typename T> typename Deque<T>::const_iterator Deque<T>::cbegin() const noexcept {
  return const_iterator(this, 0);
}

template <typename T> typename Deque<T>::const_iterator Deque<T>::cend() const noexcept {
  return const_iterator(this, size_);
}

template <typename T> void Deque<T>::ensure_capacity_for_one_more() {
  if (size_ < capacity_)
    return;
  const size_type next = capacity_ == 0 ? 1 : (capacity_ * 2);
  grow(next);
}

template <typename T> void Deque<T>::grow(size_type new_capacity) {
  T* next = alloc_.allocate(new_capacity);

  if constexpr (std::is_nothrow_move_constructible_v<T>) {
    for (size_type i = 0; i < size_; ++i)
      std::construct_at(next + i, std::move((*this)[i]));
  } else {
    for (size_type i = 0; i < size_; ++i)
      std::construct_at(next + i, (*this)[i]);
  }

  destroy_all();
  deallocate();
  data_ = next;
  capacity_ = new_capacity;
  head_ = 0;
}

template <typename T> void Deque<T>::destroy_all() noexcept {
  for (size_type i = 0; i < size_; ++i)
    std::destroy_at(data_ + phys_index(i));
}

template <typename T> void Deque<T>::deallocate() noexcept {
  if (!data_)
    return;
  alloc_.deallocate(data_, capacity_);
  data_ = nullptr;
  capacity_ = 0;
}
