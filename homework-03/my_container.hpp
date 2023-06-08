#pragma once

#include <bits/allocator.h>

namespace homework_03 {

template <typename T>
struct MyNode {
    T value;
    MyNode* next = nullptr;
};

template <typename T>
struct MyIterator {
  using value_type = T;
  using node_type = MyNode<T>;
  using iterator_type = MyIterator<T>;

  MyIterator() = default;

  MyIterator(node_type* node)
    : node_(node) {}

  value_type& operator*() const {
    return node_->value;
  }

  value_type* operator->() const {
    return &node_->value;
  }

  iterator_type& operator++() {
    node_ = node_->next;
    return *this;
  }

  iterator_type operator++(int) {
    iterator_type res = *this;
    node_ = node_->next;
    return res;
  }

  bool operator==(const iterator_type& other) const noexcept {
    if (this == &other) {
        return true;
    }

    return node_ == other.node_;
  }

  bool operator!=(const iterator_type& other) const noexcept {
    return !iterator_type::operator==(other);
  }

  node_type* node_ = nullptr;
};

template <typename T>
struct MyContIterator {
  using value_type = T;
  using node_type = MyNode<T>;
  using iterator_type = MyContIterator<T>;

  MyContIterator() = default;
  
  MyContIterator(node_type* node)
    : node_(node) {}

  const value_type& operator*() const {
    return node_->value;
  }

  const value_type* operator->() const {
    return &node_->value;
  }

  iterator_type& operator++() {
    node_ = node_->next;
    return *this;
  }

  iterator_type operator++(int) {
    iterator_type res = *this;
    node_ = node_->next;
    return res;
  }

  bool operator==(const iterator_type& other) const noexcept {
    if (this == &other) {
        return true;
    }

    return node_ == other.node_;
  }

  bool operator!=(const iterator_type& other) const noexcept {
    return !iterator_type::operator==(other);
  }

  node_type* node_ = nullptr;
};

template <typename T, typename Alloc = std::allocator<T>>
class MyContainer {
 public:
  using value_type = T;
  using allocator = Alloc;
  using node_type = MyNode<T>;
  using node_allocator = typename allocator::rebind<node_type>::other;
  using iterator = MyIterator<T>;
  using const_iterator = MyContIterator<T>;

  MyContainer() = default;

  MyContainer(const MyContainer& my_container)
    : allocator_(my_container.allocator_)
    , size_(my_container.size_) {
    for (auto node = my_container.head_; node; node = node->next) {
      node_type* new_node = std::allocator_traits<node_type>::allocate(allocator_, 1);
      if (last_) {
        (last_)->next = new_node;
      }

      if (!head_) {
        head_ = new_node;
      }

      std::allocator_traits<node_type>::construct(allocator_, new_node, *node);
      last_ = new_node;
    }
  }

  MyContainer(MyContainer&& my_container) = default;

  ~MyContainer() {
    clear();
  }

  void push_back(const T& value) {
    node_type* new_node = std::allocator_traits<node_allocator>::allocate(allocator_, 1);
    new_node->value = value;
    if (!head_) {
      head_ = new_node;
    }

    if (last_) {
      last_->next = new_node;
    }

    last_ = new_node;
  }

  void clear() {
    for (auto node = head_; node; ) {
      auto* next_node = node->next;
      node->value.~T();
      std::allocator_traits<node_allocator>::destroy(allocator_, node);
      node = next_node;
    }

    size_ = 0;
    head_ = last_ = nullptr;
  }

  std::size_t size() const noexcept {
    return size_;
  }

  iterator begin() noexcept {
    return iterator(head_);
  }

  iterator end() noexcept {
    return iterator(nullptr);
  }

  const_iterator cbegin() noexcept {
    return const_iterator(head_);
  }

  iterator cend() noexcept {
    return const_iterator(nullptr);
  }

 private:
  node_allocator allocator_;
  std::size_t size_ = 0;
  node_type* head_ = nullptr;
  node_type* last_ = nullptr;
};

}