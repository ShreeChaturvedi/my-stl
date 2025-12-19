template <typename T> List<T>::List() noexcept : sentinel_{}, size_(0) {
  sentinel_.next = &sentinel_;
  sentinel_.prev = &sentinel_;
}

template <typename T> List<T>::List(const List& other) : List() {
  for (const auto& v : other)
    push_back(v);
}

template <typename T> List<T>::List(List&& other) noexcept : List() {
  steal(std::move(other));
}

template <typename T> List<T>::~List() {
  clear();
}

template <typename T> List<T>& List<T>::operator=(const List& other) {
  if (this == &other)
    return *this;
  List tmp(other);
  *this = std::move(tmp);
  return *this;
}

template <typename T> List<T>& List<T>::operator=(List&& other) noexcept {
  if (this == &other)
    return *this;
  clear();
  steal(std::move(other));
  return *this;
}

template <typename T> bool List<T>::empty() const noexcept {
  return size_ == 0;
}

template <typename T> std::size_t List<T>::size() const noexcept {
  return size_;
}

template <typename T> void List<T>::clear() noexcept {
  NodeBase* it = sentinel_.next;
  sentinel_.next = &sentinel_;
  sentinel_.prev = &sentinel_;
  size_ = 0;
  while (it != &sentinel_) {
    NodeBase* next = it->next;
    delete as_node(it);
    it = next;
  }
}

template <typename T> T& List<T>::front() {
  return as_node(sentinel_.next)->value;
}

template <typename T> const T& List<T>::front() const {
  return as_node(sentinel_.next)->value;
}

template <typename T> T& List<T>::back() {
  return as_node(sentinel_.prev)->value;
}

template <typename T> const T& List<T>::back() const {
  return as_node(sentinel_.prev)->value;
}

template <typename T> typename List<T>::iterator List<T>::begin() noexcept {
  return iterator(sentinel_.next);
}

template <typename T> typename List<T>::const_iterator List<T>::begin() const noexcept {
  return const_iterator(sentinel_.next);
}

template <typename T> typename List<T>::const_iterator List<T>::cbegin() const noexcept {
  return const_iterator(sentinel_.next);
}

template <typename T> typename List<T>::iterator List<T>::end() noexcept {
  return iterator(&sentinel_);
}

template <typename T> typename List<T>::const_iterator List<T>::end() const noexcept {
  return const_iterator(&sentinel_);
}

template <typename T> typename List<T>::const_iterator List<T>::cend() const noexcept {
  return const_iterator(&sentinel_);
}

template <typename T> typename List<T>::iterator List<T>::push_front(const T& value) {
  return push_front(T(value));
}

template <typename T> typename List<T>::iterator List<T>::push_front(T&& value) {
  auto* node = new Node(std::move(value));
  link_between(node, &sentinel_, sentinel_.next);
  ++size_;
  return iterator(node);
}

template <typename T> typename List<T>::iterator List<T>::push_back(const T& value) {
  return push_back(T(value));
}

template <typename T> typename List<T>::iterator List<T>::push_back(T&& value) {
  auto* node = new Node(std::move(value));
  link_between(node, sentinel_.prev, &sentinel_);
  ++size_;
  return iterator(node);
}

template <typename T> void List<T>::pop_front() {
  erase(begin());
}

template <typename T> void List<T>::pop_back() {
  iterator last(sentinel_.prev);
  erase(last);
}

template <typename T> typename List<T>::iterator List<T>::erase(iterator pos) {
  NodeBase* node = pos.node_;
  NodeBase* next = node->next;
  unlink(node);
  delete as_node(node);
  --size_;
  return iterator(next);
}

template <typename T> void List<T>::move_to_front(iterator pos) {
  NodeBase* node = pos.node_;
  if (node == sentinel_.next)
    return;
  unlink(node);
  link_between(node, &sentinel_, sentinel_.next);
}

template <typename T>
void List<T>::link_between(NodeBase* node, NodeBase* before, NodeBase* after) noexcept {
  node->prev = before;
  node->next = after;
  before->next = node;
  after->prev = node;
}

template <typename T> void List<T>::unlink(NodeBase* node) noexcept {
  node->prev->next = node->next;
  node->next->prev = node->prev;
}

template <typename T> void List<T>::steal(List&& other) noexcept {
  if (other.size_ == 0)
    return;
  sentinel_.next = std::exchange(other.sentinel_.next, &other.sentinel_);
  sentinel_.prev = std::exchange(other.sentinel_.prev, &other.sentinel_);
  sentinel_.next->prev = &sentinel_;
  sentinel_.prev->next = &sentinel_;
  size_ = std::exchange(other.size_, 0);
}
