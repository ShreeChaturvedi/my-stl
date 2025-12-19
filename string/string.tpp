template <typename CharT>
basic_string<CharT>::basic_string() noexcept : data_(sso_), size_(0), capacity_(sso_capacity_) {
  sso_[0] = CharT{};
}

template <typename CharT>
basic_string<CharT>::basic_string(const CharT* s)
    : basic_string(std::basic_string_view<CharT>(s)) {}

template <typename CharT>
basic_string<CharT>::basic_string(std::basic_string_view<CharT> sv) : basic_string() {
  reserve(sv.size());
  std::copy_n(sv.data(), sv.size(), data_);
  size_ = sv.size();
  data_[size_] = CharT{};
}

template <typename CharT>
basic_string<CharT>::basic_string(const basic_string& other) : basic_string(other.view()) {}

template <typename CharT>
basic_string<CharT>::basic_string(basic_string&& other) noexcept : basic_string() {
  if (other.is_sso()) {
    *this = other;
    other.clear();
    return;
  }

  data_ = std::exchange(other.data_, other.sso_);
  size_ = std::exchange(other.size_, 0);
  capacity_ = std::exchange(other.capacity_, other.sso_capacity_);
  other.sso_[0] = CharT{};
}

template <typename CharT> basic_string<CharT>::~basic_string() {
  if (!is_sso())
    alloc_.deallocate(data_, capacity_ + 1);
}

template <typename CharT>
basic_string<CharT>& basic_string<CharT>::operator=(const basic_string& other) {
  if (this == &other)
    return *this;
  basic_string tmp(other);
  *this = std::move(tmp);
  return *this;
}

template <typename CharT>
basic_string<CharT>& basic_string<CharT>::operator=(basic_string&& other) noexcept {
  if (this == &other)
    return *this;
  if (!is_sso())
    alloc_.deallocate(data_, capacity_ + 1);
  set_sso_empty();

  if (other.is_sso()) {
    reserve(other.size_);
    std::copy_n(other.data_, other.size_, data_);
    size_ = other.size_;
    data_[size_] = CharT{};
    other.clear();
    return *this;
  }

  data_ = std::exchange(other.data_, other.sso_);
  size_ = std::exchange(other.size_, 0);
  capacity_ = std::exchange(other.capacity_, other.sso_capacity_);
  other.sso_[0] = CharT{};
  return *this;
}

template <typename CharT> void basic_string<CharT>::set_sso_empty() noexcept {
  data_ = sso_;
  size_ = 0;
  capacity_ = sso_capacity_;
  sso_[0] = CharT{};
}

template <typename CharT> void basic_string<CharT>::clear() noexcept {
  size_ = 0;
  data_[0] = CharT{};
}

template <typename CharT> bool basic_string<CharT>::empty() const noexcept {
  return size_ == 0;
}

template <typename CharT>
typename basic_string<CharT>::size_type basic_string<CharT>::size() const noexcept {
  return size_;
}

template <typename CharT>
typename basic_string<CharT>::size_type basic_string<CharT>::capacity() const noexcept {
  return capacity_;
}

template <typename CharT>
typename basic_string<CharT>::const_pointer basic_string<CharT>::c_str() const noexcept {
  return data_;
}

template <typename CharT>
typename basic_string<CharT>::const_pointer basic_string<CharT>::data() const noexcept {
  return data_;
}

template <typename CharT>
typename basic_string<CharT>::pointer basic_string<CharT>::data() noexcept {
  return data_;
}

template <typename CharT>
typename basic_string<CharT>::iterator basic_string<CharT>::begin() noexcept {
  return data_;
}

template <typename CharT>
typename basic_string<CharT>::iterator basic_string<CharT>::end() noexcept {
  return data_ + size_;
}

template <typename CharT>
typename basic_string<CharT>::const_iterator basic_string<CharT>::begin() const noexcept {
  return data_;
}

template <typename CharT>
typename basic_string<CharT>::const_iterator basic_string<CharT>::end() const noexcept {
  return data_ + size_;
}

template <typename CharT>
typename basic_string<CharT>::const_iterator basic_string<CharT>::cbegin() const noexcept {
  return data_;
}

template <typename CharT>
typename basic_string<CharT>::const_iterator basic_string<CharT>::cend() const noexcept {
  return data_ + size_;
}

template <typename CharT>
typename basic_string<CharT>::reference basic_string<CharT>::operator[](size_type i) noexcept {
  return data_[i];
}

template <typename CharT>
typename basic_string<CharT>::const_reference
basic_string<CharT>::operator[](size_type i) const noexcept {
  return data_[i];
}

template <typename CharT>
typename basic_string<CharT>::reference basic_string<CharT>::at(size_type i) {
  if (i >= size_)
    throw std::out_of_range("basic_string::at out of range");
  return data_[i];
}

template <typename CharT>
typename basic_string<CharT>::const_reference basic_string<CharT>::at(size_type i) const {
  if (i >= size_)
    throw std::out_of_range("basic_string::at out of range");
  return data_[i];
}

template <typename CharT> typename basic_string<CharT>::reference basic_string<CharT>::front() {
  if (empty())
    throw std::out_of_range("basic_string::front on empty");
  return data_[0];
}

template <typename CharT>
typename basic_string<CharT>::const_reference basic_string<CharT>::front() const {
  if (empty())
    throw std::out_of_range("basic_string::front on empty");
  return data_[0];
}

template <typename CharT> typename basic_string<CharT>::reference basic_string<CharT>::back() {
  if (empty())
    throw std::out_of_range("basic_string::back on empty");
  return data_[size_ - 1];
}

template <typename CharT>
typename basic_string<CharT>::const_reference basic_string<CharT>::back() const {
  if (empty())
    throw std::out_of_range("basic_string::back on empty");
  return data_[size_ - 1];
}

template <typename CharT> void basic_string<CharT>::reserve(size_type new_capacity) {
  if (new_capacity <= capacity_)
    return;
  reallocate(new_capacity);
}

template <typename CharT> void basic_string<CharT>::ensure_capacity_for_one_more() {
  if (size_ < capacity_)
    return;
  const size_type next = capacity_ == 0 ? 1 : (capacity_ + (capacity_ >> 1));
  reallocate(next);
}

template <typename CharT> void basic_string<CharT>::reallocate(size_type new_capacity) {
  pointer next = alloc_.allocate(new_capacity + 1);
  std::copy_n(data_, size_ + 1, next);
  if (!is_sso())
    alloc_.deallocate(data_, capacity_ + 1);
  data_ = next;
  capacity_ = new_capacity;
}

template <typename CharT> void basic_string<CharT>::push_back(CharT ch) {
  ensure_capacity_for_one_more();
  data_[size_] = ch;
  ++size_;
  data_[size_] = CharT{};
}

template <typename CharT>
basic_string<CharT>& basic_string<CharT>::append(std::basic_string_view<CharT> sv) {
  reserve(size_ + sv.size());
  std::copy_n(sv.data(), sv.size(), data_ + size_);
  size_ += sv.size();
  data_[size_] = CharT{};
  return *this;
}

template <typename CharT>
basic_string<CharT>& basic_string<CharT>::operator+=(std::basic_string_view<CharT> sv) {
  return append(sv);
}

template <typename CharT>
basic_string<CharT>& basic_string<CharT>::operator+=(const basic_string& other) {
  return append(other.view());
}

template <typename CharT> basic_string<CharT>& basic_string<CharT>::operator+=(CharT ch) {
  push_back(ch);
  return *this;
}

template <typename CharT> std::basic_string_view<CharT> basic_string<CharT>::view() const noexcept {
  return std::basic_string_view<CharT>(data_, size_);
}
