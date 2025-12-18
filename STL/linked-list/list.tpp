#include "list.h"

#include <cstddef>
#include <stdexcept>

template <typename T>
LinkedList<T>::LinkedList() : m_Sentinel(new Node), m_End(m_Sentinel), m_Size(0) {}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList& other) : LinkedList() {
  Node* it = other.m_Sentinel->next;
  while (it) {
    append(it->data);
    it = it->next;
  }
}

template <typename T>
LinkedList<T>::LinkedList(LinkedList&& other) noexcept
    : m_Sentinel(std::exchange(other.m_Sentinel, nullptr)),
      m_End(std::exchange(other.m_End, nullptr)),
      m_Size(std::exchange(other.m_Size, 0)) {}

template <typename T>
LinkedList<T>::~LinkedList() {
  if (!m_Sentinel) return;
  Node* it = m_Sentinel;
  while (it) {
    Node* nextNode = it->next;
    delete it;
    it = nextNode;
  }
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other) {
  if (this == &other) return *this;
  LinkedList tmp(other);
  *this = std::move(tmp);
  return *this;
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList&& other) noexcept {
  if (this == &other) return *this;
  if (m_Sentinel) {
    Node* it = m_Sentinel;
    while (it) {
      Node* nextNode = it->next;
      delete it;
      it = nextNode;
    }
  }
  m_Sentinel = std::exchange(other.m_Sentinel, nullptr);
  m_End = std::exchange(other.m_End, nullptr);
  m_Size = std::exchange(other.m_Size, 0);
  return *this;
}

template <typename T>
std::size_t LinkedList<T>::size() const noexcept {
  return m_Size;
}

template <typename T>
bool LinkedList<T>::empty() const noexcept {
  return m_Size == 0;
}

template <typename T>
std::size_t LinkedList<T>::append(T element) {
  m_End->next = new Node(element, nullptr);
  m_End = m_End->next;
  return ++m_Size;
}

template <typename T>
std::size_t LinkedList<T>::insert(T element, std::size_t index) {
  if (index > m_Size) throw std::invalid_argument("Supplied index out of bounds");

  Node* prev = node_before(index);
  Node* next = prev->next;
  prev->next = new Node(element, next);
  if (next == nullptr) m_End = prev->next;
  return ++m_Size;
}

template <typename T>
std::size_t LinkedList<T>::remove(std::size_t index) {
  if (index >= m_Size) throw std::invalid_argument("Supplied index out of bounds");

  Node* prev = node_before(index);
  Node* victim = prev->next;
  prev->next = victim->next;
  if (victim == m_End) m_End = prev;
  delete victim;
  return --m_Size;
}

template <typename T>
T& LinkedList<T>::operator[](std::size_t index) {
  return const_cast<T&>(std::as_const(*this)[index]);
}

template <typename T>
const T& LinkedList<T>::operator[](std::size_t index) const {
  if (index >= m_Size) throw std::invalid_argument("Supplied index out of bounds");
  Node* it = m_Sentinel->next;
  for (std::size_t pos = 0; pos < index; ++pos) it = it->next;
  return it->data;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::node_before(std::size_t index) const {
  if (index > m_Size) throw std::invalid_argument("Supplied index out of bounds");
  if (index == m_Size) return m_End;

  Node* result = m_Sentinel;
  for (std::size_t pos = 0; pos < index; ++pos) result = result->next;
  return result;
}
