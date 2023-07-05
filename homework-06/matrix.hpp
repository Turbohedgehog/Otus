#include <array>
#include <map>
#include <tuple>

namespace homework_06 {

template <typename T, T DefaulValue, std::size_t Dimension>
class Matrix;

template<typename Array, std::size_t... I>
auto a2t_impl(const Array& a, std::index_sequence<I...>)
{
    return std::make_tuple(a[I]...);
}
 
template<typename T, std::size_t N, typename Indices = std::make_index_sequence<N>>
auto a2t(const std::array<T, N>& a)
{
    return a2t_impl(a, Indices{});
}

template <typename T, T DefaulValue, std::size_t Dimension>
class ValueType {
 public:
  using value_type = T;
  using matrix_type = Matrix<T, DefaulValue, Dimension>;
  static constexpr T kDefaultValue = DefaulValue;
  static constexpr std::size_t kDimension = Dimension;
  using index_type = std::array<std::size_t, kDimension>;
};

template <typename T, T DefaulValue, std::size_t Dimension, std::size_t CurrentDimension>
class ValueHolderViewBase : public ValueType<T, DefaulValue, Dimension> {
 public:
  static constexpr std::size_t kCurrentDimension = CurrentDimension;

  ValueHolderViewBase(matrix_type& matrix, std::size_t index)
    : matrix_(matrix) {
      address_[kCurrentDimension - 1] = index;
    }

  ValueHolderViewBase(matrix_type& matrix, const index_type& address, std::size_t index)
    : ValueHolderViewBase(matrix, index) {
      address_ = address;
      address_[kCurrentDimension - 1] = index;
    }

 protected:
  matrix_type& matrix_;
  mutable index_type address_ = { 0 };
};

template <typename T, T DefaulValue, std::size_t Dimension>
class ValueHolder : public ValueType<T, DefaulValue, Dimension> {
 public:
  using value_holder_view_base = ValueHolderViewBase<T, DefaulValue, Dimension, Dimension>;

  class ValueHolderView : public value_holder_view_base {
   public:
    ValueHolderView(matrix_type& matrix, size_t index)
      : value_holder_view_base(matrix, index)
    {}

    ValueHolderView(matrix_type& matrix, const index_type& address, std::size_t index)
      : value_holder_view_base(matrix, address, index)
    {}

    const ValueHolderView& operator[](std::size_t index) const {
      address_[kDimension - 1] = index;

      return *this;
    }

    ValueHolderView& operator=(ValueHolderView& other) {
      if (this == other) {
        return *this;
      }

      *this = static_cast<T>(other);

      return *this;
    }

    ValueHolderView& operator=(T&& value) {
      matrix_.set(address_, std::forward<T>(value));

      return *this;
    }

    ValueHolderView& operator=(const T& value) {
      matrix_.set(address_, value);

      return *this;
    }

    operator T() const {
      return matrix_.get(address_);
    }
  };

  using value_holder_view_type = ValueHolderView;

  ValueHolderView at(matrix_type& matrix, std::size_t index) {
    return ValueHolderView(matrix, index);
  }

  int set(const index_type& index, const T& value) {
    std::size_t key = index[kDimension - 1];
    auto it = values_.find(key);
    if (it == values_.end()) {
      if (value == DefaulValue) {
        return 0;
      }

      values_.emplace(key, value);

      return 1;
    }

    if (value == DefaulValue) {
      values_.erase(it);

      return -1;
    }

    it->second = value;

    return 0;
  }

  int set(const index_type& index, T&& value) {
    std::size_t key = index[kDimension - 1];
    auto it = values_.find(key);
    if (it == values_.end()) {
      if (value == DefaulValue) {
        return 0;
      }

      values_.emplace(key, std::forward<T>(value));

      return 1;
    }

    if (value == DefaulValue) {
      values_.erase(it);

      return -1;
    }

    it->second = std::forward<T>(value);

    return 0;
  }

  T get(const index_type& index) const {
    std::size_t key = index[kDimension - 1];
    auto it = values_.find(key);

    return it == values_.end() ? kDefaultValue : it->second;
  }

  bool empty() const {
    return values_.empty();
  }

  bool get_first_index(index_type& index) const {
    if (values_.empty()) {
      return false;
    }

    auto it = values_.begin();
    index[kDimension - 1] = it->first;

    return true;
  }

  bool get_next_index(index_type& index) const {
    std::size_t key = index[kDimension - 1];
    auto it = values_.find(key);
    if (it == values_.end()) {
      return false;
    }

    ++it;

    if (it == values_.end()) {
      return false;
    }

    index[kDimension - 1] = it->first;

    return true;
  }

 private:
  std::map<std::size_t, T> values_;
};

template <typename T, T DefaulValue, std::size_t Dimension, std::size_t CurrentDimension>
class MatrixHolder : public ValueType<T, DefaulValue, Dimension> {
 public:
  static constexpr std::size_t kCurrentDimension = CurrentDimension;
  using value_holder = ValueHolder<T, DefaulValue, Dimension>;
  using value_holder_view_base = ValueHolderViewBase<T, DefaulValue, Dimension, CurrentDimension>;

  using sub_holder_type = std::conditional_t<
    Dimension - CurrentDimension <= 1,
    ValueHolder<T, DefaulValue, Dimension>,
    MatrixHolder<T, DefaulValue, Dimension, CurrentDimension + 1>>;
  
  class ValueHolderView : public value_holder_view_base {
   public:
    ValueHolderView(matrix_type& matrix, std::size_t index)
      : value_holder_view_base(matrix, index)
    {}

    ValueHolderView(matrix_type& matrix, const index_type& address, std::size_t index)
      : value_holder_view_base(matrix, address, index)
    {}

    typename sub_holder_type::value_holder_view_type operator[](std::size_t index) {
      return sub_holder_type::value_holder_view_type(matrix_, address_, index);
    }
  };

  using value_holder_view_type = ValueHolderView;

  ValueHolderView at(matrix_type& matrix, std::size_t index) {
    return ValueHolderView(matrix, index);
  }

  int set(const index_type& index, const T& value) {
    std::size_t key = index[kCurrentDimension - 1];
    auto it = subholders_.find(key);
    if (it == subholders_.end()) {
      if (value == DefaulValue) {
        return 0;
      }

      return subholders_[key].set(index, value);
    }

    auto res = it->second.set(index, value);
    if (res < 0 && it->second.empty()) {
      subholders_.erase(it);

      return res;
    }

    return res;
  }

  int set(const index_type& index, T&& value) {
    std::size_t key = index[kCurrentDimension - 1];
    auto it = subholders_.find(key);
    if (it == subholders_.end()) {
      if (value == DefaulValue) {
        return 0;
      }

      return subholders_[key].set(index, std::forward<T>(value));
    }

    auto res = it->second.set(index, std::forward<T>(value));
    if (res < 0 && it->second.empty()) {
      subholders_.erase(it);

      return res;
    }

    return 0;
  }

  T get(const index_type& index) const {
    std::size_t key = index[CurrentDimension - 1];
    auto it = subholders_.find(key);

    return it == subholders_.end() ? kDefaultValue : it->second.get(index);
  }

  bool empty() const {
    return subholders_.empty();
  }

  bool get_first_index(index_type& index) const {
    if (subholders_.empty()) {
      return false;
    }

    auto it = subholders_.begin();
    index[kCurrentDimension - 1] = it->first;
    return it->second.get_first_index(index);
  }

  bool get_next_index(index_type& index) const {
    if (subholders_.empty()) {
      return false;
    }

    std::size_t key = index[kCurrentDimension - 1];
    auto it = subholders_.find(key);
    if (it == subholders_.end()) {
      return false;
    }

    if (it->second.get_next_index(index)) {
      return true;
    }

    ++it;

    if (it == subholders_.end()) {
      return false;
    }

    index[kCurrentDimension - 1] = it->first;

    return it->second.get_first_index(index);
  }

 private:
  std::map<std::size_t, sub_holder_type> subholders_;
};

template <typename T, T DefaulValue, std::size_t Dimension>
class MatrixIterator : public ValueType<T, DefaulValue, Dimension> {
 public:
  using tuple_type = decltype(std::tuple_cat(a2t(index_type()), std::declval<std::tuple<T>>()));

  MatrixIterator(matrix_type* matrix, index_type&& index)
    : matrix_(matrix)
    , index_(std::forward<index_type>(index))
  {}

  MatrixIterator& operator++() {
    if (!matrix_) {
      return *this;
    }

    if (!matrix_->get_next_index(index_)) {
      matrix_ = nullptr;
    }

    return *this;
  }

  MatrixIterator operator++(int) {
    if (!matrix_) {
      return *this;
    }

    auto res = *this;
    if (!matrix_->get_next_index(index_)) {
      matrix_ = nullptr;
    }

    return res;
  }

  tuple_type operator*() const {
    return std::tuple_cat(a2t(index_), std::forward_as_tuple(matrix_->get(index_)));
  }

  bool operator== (const MatrixIterator& other) const {
    if (this == &other) {
      return true;
    }

    if (matrix_ != other.matrix_) {
      return false;
    }

    if (matrix_ == nullptr) {
      return true;
    }

    return index_ == other.index_;
  }

  bool operator!= (const MatrixIterator& other) const {
    return !operator==(other);
  }

 private:
  matrix_type* matrix_ = nullptr;
  index_type index_;
};

template <typename T, T DefaulValue, std::size_t Dimension>
class MatrixConstIterator : public ValueType<T, DefaulValue, Dimension> {
 public:
  using tuple_type = decltype(std::tuple_cat(a2t(index_type()), std::declval<std::tuple<T>>()));

  MatrixConstIterator(const matrix_type* matrix, index_type&& index)
    : matrix_(matrix)
    , index_(std::forward<index_type>(index))
  {}

  MatrixConstIterator& operator++() {
    if (!matrix_) {
      return *this;
    }

    if (!matrix_->get_next_index(index_)) {
      matrix_ = nullptr;
    }

    return *this;
  }

  MatrixConstIterator operator++(int) {
    if (!matrix_) {
      return *this;
    }

    auto res = *this;
    if (!matrix_->get_next_index(index_)) {
      matrix_ = nullptr;
    }

    return res;
  }

  tuple_type operator*() const {
    return std::tuple_cat(a2t(index_), std::forward_as_tuple(matrix_->get(index_)));
  }

  bool operator== (const MatrixConstIterator& other) const {
    if (this == &other) {
      return true;
    }

    if (matrix_ != other.matrix_) {
      return false;
    }

    if (matrix_ == nullptr) {
      return true;
    }

    return index_ == other.index_;
  }

  bool operator!= (const MatrixConstIterator& other) const {
    return !operator==(other);
  }

 private:
  const matrix_type* matrix_ = nullptr;
  index_type index_;
};

template <typename T, T DefaulValue = 0, std::size_t Dimension = 2>
class Matrix : public ValueType<T, DefaulValue, Dimension> {
 public:
  using matrix_holder_type = typename MatrixHolder<T, DefaulValue, Dimension, 0>::sub_holder_type;
  using matrix_holder_view_type = typename matrix_holder_type::value_holder_view_type;
  using iterator = MatrixIterator<T, DefaulValue, Dimension>;
  using const_iterator = MatrixConstIterator<T, DefaulValue, Dimension>;

  friend class iterator;
  friend class const_iterator;

  std::size_t size() const {
    return size_;
  }

  matrix_holder_view_type operator[](std::size_t key) {
    return matrix_holder_.at(*this, key);
  }

  void set(const index_type& index, const T& value) {
    auto diff = matrix_holder_.set(index, value);
    if (diff > 0) {
      ++size_;
    } else if (diff < 0) {
      --size_;
    }
  }

  void set(const index_type& index, T&& value) {
    auto diff = matrix_holder_.set(index, std::forward<T>(value));
    if (diff > 0) {
      ++size_;
    } else if (diff < 0) {
      --size_;
    }
  }

  T get(const index_type& index) const {
    return matrix_holder_.get(index);
  }

  iterator begin() {
    index_type index = { 0 };
    if (matrix_holder_.get_first_index(index)) {
      return iterator(this, std::move(index));  
    }

    return end();
  }

  iterator end() { 
    return iterator(nullptr, index_type());
  }

  const_iterator begin() const {
    index_type index = { 0 };
    if (matrix_holder_.get_first_index(index)) {
      return const_iterator(this, std::move(index));  
    }

    return end();
  }

  const_iterator end() const {
    return const_iterator(nullptr, index_type());
  }

 private:
  bool get_next_index(index_type& index) const {
    return matrix_holder_.get_next_index(index);
  }
  std::size_t size_ = 0;
  matrix_holder_type matrix_holder_;
};

}  // namespace homework_06 
