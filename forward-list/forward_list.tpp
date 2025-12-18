template <typename T>
ForwardList<T>::ForwardList() noexcept : head_{} {}

template <typename T>
ForwardList<T>::ForwardList(const ForwardList& other) : ForwardList() {
  NodeBase* tail = &head_;
  for (const auto& v : other) {
    auto* n = new Node(v);
    tail->next = n;
    tail = n;
  }
}

template <typename T>
ForwardList<T>::ForwardList(ForwardList&& other) noexcept : ForwardList() {
  steal(std::move(other));
}

template <typename T>
ForwardList<T>::~ForwardList() {
  clear();
}

template <typename T>
ForwardList<T>& ForwardList<T>::operator=(const ForwardList& other) {
  if (this == &other) return *this;
  ForwardList tmp(other);
  *this = std::move(tmp);
  return *this;
}

template <typename T>
ForwardList<T>& ForwardList<T>::operator=(ForwardList&& other) noexcept {
  if (this == &other) return *this;
  clear();
  steal(std::move(other));
  return *this;
}

template <typename T>
bool ForwardList<T>::empty() const noexcept {
  return head_.next == nullptr;
}

template <typename T>
void ForwardList<T>::clear() noexcept {
  NodeBase* it = head_.next;
  head_.next = nullptr;
  while (it) {
    NodeBase* next = it->next;
    delete as_node(it);
    it = next;
  }
}

template <typename T>
T& ForwardList<T>::front() {
  return as_node(head_.next)->value;
}

template <typename T>
const T& ForwardList<T>::front() const {
  return as_node(head_.next)->value;
}

template <typename T>
void ForwardList<T>::push_front(const T& value) {
  (void)emplace_front(value);
}

template <typename T>
void ForwardList<T>::push_front(T&& value) {
  (void)emplace_front(std::move(value));
}

template <typename T>
template <typename... Args>
T& ForwardList<T>::emplace_front(Args&&... args) {
  auto* n = new Node(std::forward<Args>(args)...);
  n->next = head_.next;
  head_.next = n;
  return n->value;
}

template <typename T>
typename ForwardList<T>::iterator ForwardList<T>::before_begin() noexcept {
  return iterator(&head_);
}

template <typename T>
typename ForwardList<T>::const_iterator ForwardList<T>::before_begin() const noexcept {
  return const_iterator(&head_);
}

template <typename T>
typename ForwardList<T>::iterator ForwardList<T>::begin() noexcept {
  return iterator(head_.next);
}

template <typename T>
typename ForwardList<T>::const_iterator ForwardList<T>::begin() const noexcept {
  return const_iterator(head_.next);
}

template <typename T>
typename ForwardList<T>::const_iterator ForwardList<T>::cbegin() const noexcept {
  return const_iterator(head_.next);
}

template <typename T>
typename ForwardList<T>::iterator ForwardList<T>::end() noexcept {
  return iterator(nullptr);
}

template <typename T>
typename ForwardList<T>::const_iterator ForwardList<T>::end() const noexcept {
  return const_iterator(nullptr);
}

template <typename T>
typename ForwardList<T>::const_iterator ForwardList<T>::cend() const noexcept {
  return const_iterator(nullptr);
}

template <typename T>
typename ForwardList<T>::iterator ForwardList<T>::erase_after(iterator pos) {
  NodeBase* before = pos.node_;
  NodeBase* victim = before->next;
  before->next = victim->next;
  delete as_node(victim);
  return iterator(before->next);
}

template <typename T>
void ForwardList<T>::steal(ForwardList&& other) noexcept {
  head_.next = std::exchange(other.head_.next, nullptr);
}

