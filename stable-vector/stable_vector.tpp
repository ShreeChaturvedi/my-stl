template <typename T>
StableVector<T>::StableVector(std::initializer_list<T> list) {
  slots_.reserve(list.size());
  for (const auto& v : list) emplace_back(v);
}

template <typename T>
StableVector<T>::StableVector(const StableVector& other) {
  slots_.reserve(other.size());
  for (const auto& v : other) emplace_back(v);
}

template <typename T>
StableVector<T>& StableVector<T>::operator=(const StableVector& other) {
  if (this == &other) return *this;
  StableVector tmp(other);
  slots_.swap(tmp.slots_);
  return *this;
}

template <typename T>
typename StableVector<T>::reference StableVector<T>::at(size_type i) {
  if (i >= size()) throw std::out_of_range("StableVector::at out of range");
  return (*this)[i];
}

template <typename T>
typename StableVector<T>::const_reference StableVector<T>::at(size_type i) const {
  if (i >= size()) throw std::out_of_range("StableVector::at out of range");
  return (*this)[i];
}

template <typename T>
typename StableVector<T>::reference StableVector<T>::front() {
  if (empty()) throw std::out_of_range("StableVector::front on empty");
  return (*this)[0];
}

template <typename T>
typename StableVector<T>::const_reference StableVector<T>::front() const {
  if (empty()) throw std::out_of_range("StableVector::front on empty");
  return (*this)[0];
}

template <typename T>
typename StableVector<T>::reference StableVector<T>::back() {
  if (empty()) throw std::out_of_range("StableVector::back on empty");
  return (*this)[size() - 1];
}

template <typename T>
typename StableVector<T>::const_reference StableVector<T>::back() const {
  if (empty()) throw std::out_of_range("StableVector::back on empty");
  return (*this)[size() - 1];
}

template <typename T>
template <typename... Args>
typename StableVector<T>::reference StableVector<T>::emplace_back(Args&&... args) {
  slots_.emplace_back(make(std::forward<Args>(args)...));
  return back();
}

template <typename T>
void StableVector<T>::pop_back() {
  if (empty()) throw std::out_of_range("StableVector::pop_back on empty");
  slots_.pop_back();
}

template <typename T>
template <typename... Args>
typename StableVector<T>::iterator StableVector<T>::emplace(const_iterator pos, Args&&... args) {
  auto it = slots_.emplace(pos.base(), make(std::forward<Args>(args)...));
  return iterator(it);
}

template <typename T>
typename StableVector<T>::iterator StableVector<T>::erase(const_iterator pos) {
  auto it = slots_.erase(pos.base());
  return iterator(it);
}

template <typename T>
typename StableVector<T>::iterator StableVector<T>::erase(const_iterator first, const_iterator last) {
  auto it = slots_.erase(first.base(), last.base());
  return iterator(it);
}

template <typename T>
typename StableVector<T>::iterator StableVector<T>::begin() noexcept {
  return iterator(slots_.begin());
}

template <typename T>
typename StableVector<T>::const_iterator StableVector<T>::begin() const noexcept {
  return const_iterator(slots_.begin());
}

template <typename T>
typename StableVector<T>::const_iterator StableVector<T>::cbegin() const noexcept {
  return const_iterator(slots_.cbegin());
}

template <typename T>
typename StableVector<T>::iterator StableVector<T>::end() noexcept {
  return iterator(slots_.end());
}

template <typename T>
typename StableVector<T>::const_iterator StableVector<T>::end() const noexcept {
  return const_iterator(slots_.end());
}

template <typename T>
typename StableVector<T>::const_iterator StableVector<T>::cend() const noexcept {
  return const_iterator(slots_.cend());
}

