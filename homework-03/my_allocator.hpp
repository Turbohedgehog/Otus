#pragma once

#include <vector>
#include <bitset>
#include <stdlib.h>
#include <exception>
#include <list>
#include <cstring>

namespace homework_03 {

template <typename T, int Size>
class MyChunkBase {
 public:
  using value_type = T;

  static constexpr std::size_t kItemSize = sizeof(T);
  static constexpr std::size_t kChunkSize = kItemSize * Size;

  MyChunkBase() {
    chunk_ = static_cast<value_type*>(malloc(MyChunkBase::kChunkSize));
    if (!chunk_) {
      throw std::bad_alloc();
    }
  }

  MyChunkBase(const MyChunkBase& other) : MyChunkBase() {
    std::memcpy(chunk_, other.chunk_, MyChunkBase::kChunkSize);
  };

  MyChunkBase(MyChunkBase&& other) {
    std::swap(chunk_, other.chunk_);
  };

  ~MyChunkBase() {
    free(chunk_);
  }

  bool Contains(const value_type* ptr) const {
    const value_type* end = chunk_ + Size;
    return ptr >= chunk_ && ptr < end;
  }

  bool IsEmpty() const {
    return allocated_count_ >= Size;
  }

 protected:
  value_type* chunk_ = nullptr;
  std::size_t allocated_count_ = 0;
};

template <typename T, int Size>
class MySimpleChunk : public MyChunkBase<T, Size> {
 public:
  T* Request(std::size_t count) {
    if (MyChunkBase<T, Size>::allocated_count_ + count > Size) {
      throw std::bad_alloc();
    }

    T* item_ptr = MyChunkBase<T, Size>::chunk_ + MyChunkBase<T, Size>::allocated_count_;
    MyChunkBase<T, Size>::allocated_count_ += count;

    return item_ptr;
  }

  void Deallocate(T* /*ptr*/, std::size_t /*count*/) {}
};

template <typename T, int Size>
class MyChunkWithDeallocation : public MyChunkBase<T, Size> {
 public:
  T* Request(std::size_t count) {
    if (count > Size) {
      throw std::bad_alloc();
    }

    std::size_t counter = 0;
    for (std::size_t bit = 0; bit < Size; ++bit) {
      if (allocated_items_.test(bit)) {
        counter = 0;
        continue;
      }

      if (++counter < count) {
        continue;
      }

      for (std::size_t i = 0; i < count; ++i) {
        allocated_items_.set(bit - i, true);
      }

      MyChunkBase<T, Size>::allocated_count_ += count;
      T* item = MyChunkBase<T, Size>::chunk_ + bit - (count - 1);
      return item;
    }
    
    return nullptr;
  }

  void Deallocate(T* ptr, std::size_t count) {
    std::ptrdiff_t range_start = ptr - MyChunkBase<T, Size>::chunk_;
    for (std::size_t idx = 0; idx < count; ++idx) {
      allocated_items_.set(range_start + idx, false);
    }

    MyChunkBase<T, Size>::allocated_count_ -= count;
  }

 private:
  std::bitset<Size> allocated_items_ = {0};
};

template <typename Chunk>
class MyBasicChunkHolder {
 public:
  typename Chunk::value_type* Request(std::size_t count) {
    auto ptr = chunk_.Request(count);
    if (!ptr) {
      throw std::bad_alloc();
    }

    return ptr;
  }

  void Deallocate(typename Chunk::value_type* ptr, std::size_t count) {
    chunk_.Deallocate(ptr, count);
  }

 private:
  Chunk chunk_;
};

template <typename Chunk>
class MyExpandableChunkHolder {
 public:
  using chunk_type = Chunk;
  using value_type = typename Chunk::value_type;

  MyExpandableChunkHolder() = default;

  MyExpandableChunkHolder(const MyExpandableChunkHolder& other) {
    for (auto* other_chunk : other.chunks_) {
      auto* chunk = new Chunk(*other_chunk);
      chunks_.push_back(chunk);
    }
  }

  MyExpandableChunkHolder(MyExpandableChunkHolder&&) = default;

  ~MyExpandableChunkHolder() {
    for (auto chunk : chunks_) {
      delete chunk;
    }
  }

  value_type* Request(std::size_t count) {
    for (auto* chunk : chunks_) {
      if (chunk->IsEmpty()) {
        continue;
      }

      value_type* ptr = chunk->Request(count);
      if (ptr) {
        return ptr;
      }
    }

    auto* chunk = new Chunk();
    chunks_.push_back(chunk);

    return chunk->Request(count);
  }

  void Deallocate(value_type* ptr, std::size_t count) {
    for (auto* chunk : chunks_) {
      if (chunk->Contains(ptr)) {
        chunk->Deallocate(ptr, count);
        return;
      }
    }
  }

 private:
  std::vector<Chunk*> chunks_;
};

template <
    typename T,
    std::size_t ChunkSize,
    bool AllowExpand = false,
    bool AllowItemsDeallocation = false>
class MyAllocator {
 public:
  using chunk_type = std::conditional_t<
      AllowItemsDeallocation,
      MyChunkWithDeallocation<T, ChunkSize>,
      MySimpleChunk<T, ChunkSize>>;

  using chunk_holder = std::conditional_t<
      AllowExpand,
      MyExpandableChunkHolder<chunk_type>,
      MyBasicChunkHolder<chunk_type>>;

  MyAllocator() = default;
  MyAllocator(const MyAllocator&) = default;
  MyAllocator(MyAllocator&&) = default;
  
  template <typename U>
	MyAllocator(const MyAllocator<U, ChunkSize, AllowExpand, AllowItemsDeallocation>&) {};
  template <typename U>
	MyAllocator(MyAllocator<U, ChunkSize, AllowExpand, AllowItemsDeallocation>&&) {};
  
 
  using value_type = T;
  using size_type = std::size_t;
  using pointer = T *;
  using const_pointer = const T *;

  static constexpr int kChunkSize = ChunkSize;
  static constexpr bool kAllowExpand = AllowExpand;
  static constexpr bool kAllowItemsDeallocation = AllowItemsDeallocation;

  using propagate_on_container_copy_assignment = std::true_type;
  using propagate_on_container_move_assignment = std::true_type;
  using propagate_on_container_swap = std::true_type;

  T* allocate(std::size_t n) {
    return chunk_holder_.Request(n);
  }

  void deallocate(T* ptr, std::size_t count) {
    chunk_holder_.Deallocate(ptr, count);
  }

  template<typename U>
  struct rebind {
    using other = MyAllocator<U, ChunkSize, AllowExpand, AllowItemsDeallocation>;
  };

 private:
  chunk_holder chunk_holder_;
};

}  // namespace homework_03
