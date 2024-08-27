#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <initializer_list>
#include <memory>

template <class T>
class VectorIterator {
  T* ptr_;

 public:
  using Iter = T*;
  using iterator_type = Iter;                                                        // NOLINT
  using iterator_category = typename std::iterator_traits<Iter>::iterator_category;  // NOLINT
  using value_type = typename std::iterator_traits<Iter>::value_type;                // NOLINT
  using difference_type = typename std::iterator_traits<Iter>::difference_type;      // NOLINT
  using pointer = typename std::iterator_traits<Iter>::pointer;                      // NOLINT
  using reference = typename std::iterator_traits<Iter>::reference;                  // NOLINT

  VectorIterator() : ptr_(nullptr) {
  }

  VectorIterator(T* ptr) : ptr_(ptr) {  // NOLINT
  }

  VectorIterator<T>& operator++() {
    ++ptr_;
    return *this;
  }

  VectorIterator<T> operator++(int) {
    auto old_it = *this;
    ++ptr_;
    return old_it;
  }

  VectorIterator<T>& operator--() {
    --ptr_;
    return *this;
  }

  VectorIterator<T> operator--(int) {
    auto old_it = *this;
    --ptr_;
    return old_it;
  }

  T& operator*() {
    return *ptr_;
  }

  T* operator->() {
    return ptr_;
  }

  bool operator==(const VectorIterator<T>& other) const {
    return ptr_ == other.ptr_;
  }

  bool operator!=(const VectorIterator<T>& other) const {
    return ptr_ != other.ptr_;
  }

  bool operator<(const VectorIterator<T>& other) const {
    return ptr_ < other.ptr_;
  }

  bool operator>(const VectorIterator<T>& other) const {
    return ptr_ > other.ptr_;
  }

  bool operator<=(const VectorIterator<T>& other) const {
    return ptr_ <= other.ptr_;
  }

  bool operator>=(const VectorIterator<T>& other) const {
    return ptr_ >= other.ptr_;
  }

  VectorIterator<T> operator+(int n) const {
    return VectorIterator<T>(ptr_ + n);
  }

  VectorIterator<T>& operator+=(int n) {
    ptr_ += n;
    return *this;
  }

  VectorIterator<T> operator-(int n) const {
    return VectorIterator<T>(ptr_ - n);
  }

  VectorIterator<T>& operator-=(int n) {
    ptr_ -= n;
    return *this;
  }

  int operator-(const VectorIterator<T>& other) {
    return ptr_ - other.ptr_;
  }

  T& operator[](int idx) {
    return *(ptr_ + idx);
  }
};

template <class T>
class ConstVectorIterator {
  const T* ptr_;

 public:
  using Iter = const T*;
  using iterator_type = Iter;                                                        // NOLINT
  using iterator_category = typename std::iterator_traits<Iter>::iterator_category;  // NOLINT
  using value_type = typename std::iterator_traits<Iter>::value_type;                // NOLINT
  using difference_type = typename std::iterator_traits<Iter>::difference_type;      // NOLINT
  using pointer = typename std::iterator_traits<Iter>::pointer;                      // NOLINT
  using reference = typename std::iterator_traits<Iter>::reference;                  // NOLINT

  ConstVectorIterator() : ptr_(nullptr) {
  }

  ConstVectorIterator(T* ptr) : ptr_(ptr) {  // NOLINT
  }

  ConstVectorIterator<T>& operator++() {
    ++ptr_;
    return *this;
  }

  ConstVectorIterator<T> operator++(int) {
    auto old_it = *this;
    ++ptr_;
    return old_it;
  }

  ConstVectorIterator<T>& operator--() {
    --ptr_;
    return *this;
  }

  ConstVectorIterator<T> operator--(int) {
    auto old_it = *this;
    --ptr_;
    return old_it;
  }

  const T& operator*() {
    return *ptr_;
  }

  T* operator->() {
    return ptr_;
  }

  bool operator==(const ConstVectorIterator<T>& other) const {
    return ptr_ == other.ptr_;
  }

  bool operator!=(const ConstVectorIterator<T>& other) const {
    return ptr_ != other.ptr_;
  }

  bool operator<(const ConstVectorIterator<T>& other) const {
    return ptr_ < other.ptr_;
  }

  bool operator>(const ConstVectorIterator<T>& other) const {
    return ptr_ > other.ptr_;
  }

  bool operator<=(const ConstVectorIterator<T>& other) const {
    return ptr_ <= other.ptr_;
  }

  bool operator>=(const ConstVectorIterator<T>& other) const {
    return ptr_ >= other.ptr_;
  }

  ConstVectorIterator<T> operator+(int n) const {
    return ConstVectorIterator<T>(ptr_ + n);
  }

  const ConstVectorIterator<T>& operator+=(int n) {
    ptr_ += n;
    return *this;
  }

  ConstVectorIterator<T> operator-(int n) const {
    return ConstVectorIterator<T>(ptr_ - n);
  }

  const ConstVectorIterator<T>& operator-=(int n) {
    ptr_ -= n;
    return *this;
  }

  int operator-(const ConstVectorIterator<T>& other) {
    return ptr_ - other.ptr_;
  }

  const T& operator[](int idx) {
    return *(ptr_ - idx);
  }
};

template <class T>
class Vector {
  T* data_;
  size_t size_;
  size_t capacity_;

 public:
  using ValueType = T;
  using Pointer = T*;
  using ConstPointer = const T*;
  using Reference = T&;
  using ConstReference = const T&;
  using SizeType = size_t;
  using Iterator = VectorIterator<T>;
  using ConstIterator = ConstVectorIterator<T>;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

  Vector() : data_(nullptr), size_(0), capacity_(0) {
  }

  explicit Vector(size_t size)
      : data_(size != 0 ? static_cast<T*>(operator new(sizeof(T) * size)) : nullptr), size_(size), capacity_(size) {
    for (size_t i = 0; i < size; ++i) {
      try {
        new (data_ + i) T();
      } catch (...) {
        for (size_t j = 0; j < i; ++j) {
          (data_ + j)->~T();
        }
        operator delete(data_);
        data_ = nullptr;
        capacity_ = size_ = 0;
        throw;
      }
    }
  }

  Vector(size_t size, T value)
      : data_(size != 0 ? static_cast<T*>(operator new(sizeof(T) * size)) : nullptr), size_(size), capacity_(size) {
    for (size_t i = 0; i < size; ++i) {
      try {
        new (data_ + i) T(value);
      } catch (...) {
        for (size_t j = 0; j < i; ++j) {
          (data_ + j)->~T();
        }
        operator delete(data_);
        data_ = nullptr;
        capacity_ = size_ = 0;
        throw;
      }
    }
  }

  template <class Iterator,
            class = std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag,
                                                       typename std::iterator_traits<Iterator>::iterator_category> > >
  Vector(Iterator first, Iterator last)
      : data_(last - first != 0 ? static_cast<T*>(operator new(sizeof(T) * (last - first))) : nullptr)
      , size_(last - first)
      , capacity_(last - first) {
    for (size_t i = 0; i < size_t(last - first); ++i) {
      try {
        new (data_ + i) T(*(first + i));
      } catch (...) {
        for (size_t j = 0; j < i; ++j) {
          (data_ + j)->~T();
        }
        operator delete(data_);
        data_ = nullptr;
        capacity_ = size_ = 0;
        throw;
      }
    }
  }

  Vector(std::initializer_list<T> list)
      : data_(list.size() != 0 ? reinterpret_cast<T*>(operator new(sizeof(T) * list.size())) : nullptr)
      , size_(list.size())
      , capacity_(list.size()) {
    try {
      std::uninitialized_copy(list.begin(), list.end(), data_);
    } catch (...) {
      operator delete(data_);
      data_ = nullptr;
      capacity_ = size_ = 0;
      throw;
    }
  }

  Vector(const Vector<T>& other)
      : data_(other.capacity_ != 0 ? static_cast<T*>(operator new(sizeof(T) * (other.capacity_))) : nullptr)
      , size_(other.size_)
      , capacity_(other.capacity_) {
    for (size_t i = 0; i < other.size_; ++i) {
      try {
        new (data_ + i) T(*const_cast<const T*>(other.data_ + i));
      } catch (...) {
        for (size_t j = 0; j < i; ++j) {
          (data_ + j)->~T();
        }
        operator delete(data_);
        data_ = nullptr;
        capacity_ = size_ = 0;
        throw;
      }
    }
  }

  Vector<T>& operator=(const Vector<T>& other) {
    if (&other != this) {
      for (size_t i = 0; i < size_; ++i) {
        (data_ + i)->~T();
      }
      operator delete(data_);
      try {
        data_ = other.capacity_ != 0 ? static_cast<T*>(operator new(sizeof(T) * (other.capacity_))) : nullptr;
      } catch (...) {
        data_ = nullptr;
        capacity_ = size_ = 0;
        throw;
      }
      size_ = other.size_;
      capacity_ = other.capacity_;
      for (size_t i = 0; i < other.size_; ++i) {
        try {
          new (data_ + i) T(*(other.data_ + i));
        } catch (...) {
          for (size_t j = 0; j < i; ++j) {
            (data_ + j)->~T();
          }
          operator delete(data_);
          data_ = nullptr;
          capacity_ = size_ = 0;
          throw;
        }
      }
    }
    return *this;
  }

  Vector(Vector<T>&& other) noexcept : data_(std::move(other.data_)), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  Vector<T>& operator=(Vector<T>&& other) noexcept {
    if (&other != this) {
      for (size_t i = 0; i < size_; ++i) {
        (data_ + i)->~T();
      }
      operator delete(data_);
      data_ = std::move(other.data_);
      size_ = other.size_;
      capacity_ = other.capacity_;
      other.data_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }
    return *this;
  }

  ~Vector() noexcept {
    for (size_t i = 0; i < size_; ++i) {
      (data_ + i)->~T();
    }
    operator delete(data_);
  }

  size_t Size() const noexcept {
    return size_;
  }

  size_t Capacity() const noexcept {
    return capacity_;
  }

  bool Empty() const noexcept {
    return size_ == 0;
  }

  T& operator[](int64_t idx) noexcept {
    return *(data_ + idx);
  }

  const T& operator[](int64_t idx) const noexcept {
    return *(data_ + idx);
  }

  T& At(int64_t idx) {
    if (idx >= int64_t(size_) || idx < 0) {
      throw std::out_of_range("Index is out of range");
    }
    return *(data_ + idx);
  }

  const T& At(int64_t idx) const {
    if (idx >= int64_t(size_) || idx < 0) {
      throw std::out_of_range("Index is out of range");
    }
    return *(data_ + idx);
  }

  T& Front() noexcept {
    return *data_;
  }

  const T& Front() const noexcept {
    return *data_;
  }

  T& Back() noexcept {
    return *(data_ + size_ - 1);
  }

  const T& Back() const noexcept {
    return *(data_ + size_ - 1);
  }

  T* Data() noexcept {
    return data_;
  }

  const T* Data() const noexcept {
    return data_;
  }

  void Swap(Vector<T>& other) {
    auto tmp = std::move(other.data_);
    other.data_ = std::move(this->data_);
    data_ = std::move(tmp);
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
  }

  void Resize(size_t new_size) {
    if (new_size <= size_) {
      for (size_t i = new_size; i < size_; ++i) {
        (data_ + i)->~T();
      }
      size_ = new_size;
    } else if (new_size <= capacity_) {
      for (size_t i = size_; i < new_size; ++i) {
        try {
          new (data_ + i) T();
        } catch (...) {
          for (size_t j = size_; j < i; ++j) {
            (data_ + j)->~T();
          }
          throw;
        }
      }
      size_ = new_size;
    } else {
      auto tmp = static_cast<T*>(operator new(sizeof(T) * new_size));
      for (size_t i = 0; i < size_; ++i) {
        new (tmp + i) T(std::move(*(data_ + i)));
      }
      for (size_t i = size_; i < new_size; ++i) {
        try {
          new (tmp + i) T();
        } catch (...) {
          for (size_t j = 0; j < size_; ++j) {
            *(data_ + j) = std::move(*(tmp + j));
          }
          for (size_t j = 0; j < i; ++j) {
            (tmp + j)->~T();
          }
          operator delete(tmp);
          throw;
        }
      }
      for (size_t i = 0; i < size_; ++i) {
        (data_ + i)->~T();
      }
      operator delete(data_);
      data_ = std::move(tmp);
      capacity_ = size_ = new_size;
    }
  }

  void Resize(size_t new_size, T value) {
    if (new_size < size_) {
      for (size_t i = new_size; i < size_; ++i) {
        (data_ + i)->~T();
      }
      size_ = new_size;
    } else if (new_size <= capacity_) {
      for (size_t i = size_; i < new_size; ++i) {
        try {
          new (data_ + i) T(value);
        } catch (...) {
          for (size_t j = size_; j < i; ++j) {
            (data_ + j)->~T();
          }
          throw;
        }
        size_ = new_size;
      }
    } else {
      auto tmp = static_cast<T*>(operator new(sizeof(T) * new_size));
      for (size_t i = 0; i < size_; ++i) {
        new (tmp + i) T(std::move(*(data_ + i)));
      }
      for (size_t i = size_; i < new_size; ++i) {
        try {
          new (tmp + i) T(value);
        } catch (...) {
          for (size_t j = 0; j < size_; ++j) {
            *(data_ + j) = std::move(*(tmp + j));
          }
          for (size_t j = 0; j < i; ++j) {
            (tmp + j)->~T();
          }
          operator delete(tmp);
          throw;
        }
      }
      for (size_t i = 0; i < size_; ++i) {
        (data_ + i)->~T();
      }
      operator delete(data_);
      data_ = std::move(tmp);
      capacity_ = size_ = new_size;
    }
  }

  void Reserve(size_t new_cap) {
    if (capacity_ >= new_cap) {
      return;
    }
    auto tmp = static_cast<T*>(operator new(sizeof(T) * new_cap));
    for (size_t i = 0; i < size_; ++i) {
      new (tmp + i) T(std::forward<T>(*(data_ + i)));
    }
    for (size_t i = 0; i < size_; ++i) {
      (data_ + i)->~T();
    }
    operator delete(data_);
    data_ = std::move(tmp);
    capacity_ = new_cap;
  }

  void ShrinkToFit() {
    if (size_ >= capacity_) {
      return;
    }
    auto tmp = size_ == 0 ? nullptr : static_cast<T*>(operator new(sizeof(T) * size_));
    for (size_t i = 0; i < size_; ++i) {
      new (tmp + i) T(std::forward<T>(*(data_ + i)));
    }
    for (size_t i = 0; i < size_; ++i) {
      (data_ + i)->~T();
    }
    operator delete(data_);
    data_ = std::move(tmp);
    capacity_ = size_;
  }

  void Clear() noexcept {
    for (size_t i = 0; i < size_; ++i) {
      (data_ + i)->~T();
    }
    size_ = 0;
  }

  void PushBack(const T& new_element) {
    if (capacity_ == size_) {
      auto tmp = static_cast<T*>(operator new(sizeof(T) * (capacity_ == 0 ? 1 : capacity_ * 2)));
      for (size_t i = 0; i < size_; ++i) {
        new (tmp + i) T(std::move(*(data_ + i)));
      }
      try {
        new (tmp + size_) T(new_element);
      } catch (...) {
        for (size_t i = 0; i < size_; ++i) {
          *(data_ + i) = std::move(*(tmp + i));
        }
        operator delete(tmp);
        throw;
      }
      for (size_t i = 0; i < size_; ++i) {
        (data_ + i)->~T();
      }
      operator delete(data_);
      data_ = std::move(tmp);
      ++size_;
      capacity_ = (capacity_ == 0 ? 1 : capacity_ * 2);
    } else {
      new (data_ + size_) T(new_element);
      ++size_;
    }
  }

  void PushBack(T&& new_element) {
    if (capacity_ == size_) {
      auto tmp = static_cast<T*>(operator new(sizeof(T) * (capacity_ == 0 ? 1 : capacity_ * 2)));
      for (size_t i = 0; i < size_; ++i) {
        new (tmp + i) T(std::move(*(data_ + i)));
      }
      new (tmp + size_) T(std::move(new_element));
      for (size_t i = 0; i < size_; ++i) {
        (data_ + i)->~T();
      }
      operator delete(data_);
      data_ = std::move(tmp);
      ++size_;
      capacity_ = (capacity_ == 0 ? 1 : capacity_ * 2);
    } else {
      new (data_ + size_) T(std::move(new_element));
      ++size_;
    }
  }

  void PopBack() noexcept {
    (data_ + size_ - 1)->~T();
    --size_;
  }

  template <class... Args>
  void EmplaceBack(Args&&... args) {
    if (capacity_ == size_) {
      auto tmp = static_cast<T*>(operator new(sizeof(T) * (capacity_ == 0 ? 1 : capacity_ * 2)));
      for (size_t i = 0; i < size_; ++i) {
        new (tmp + i) T(std::move(*(data_ + i)));
      }
      new (tmp + size_) T(std::forward<Args>(args)...);
      for (size_t i = 0; i < size_; ++i) {
        (data_ + i)->~T();
      }
      operator delete(data_);
      data_ = std::move(tmp);
      ++size_;
      capacity_ = (capacity_ == 0 ? 1 : capacity_ * 2);
    } else {
      new (data_ + size_) T(std::forward<Args>(args)...);
      ++size_;
    }
  }

  bool operator<(const Vector<T>& other) const noexcept {
    size_t min_size = std::min(size_, other.size_);
    for (size_t i = 0; i < min_size; ++i) {
      if (*(data_ + i) < *(other.data_ + i)) {
        return true;
      }
      if (*(data_ + i) > *(other.data_ + i)) {
        return false;
      }
    }
    return size_ < other.size_;
  }

  bool operator>(const Vector<T>& other) const noexcept {
    return other < *this;
  }

  bool operator==(const Vector<T>& other) const noexcept {
    if (size_ != other.size_) {
      return false;
    }
    for (size_t i = 0; i < size_; ++i) {
      if (*(data_ + i) != *(other.data_ + i)) {
        return false;
      }
    }
    return true;
  }

  bool operator<=(const Vector<T>& other) const noexcept {
    return *this == other || *this < other;
  }

  bool operator>=(const Vector<T>& other) const noexcept {
    return *this == other || *this > other;
  }

  bool operator!=(const Vector<T>& other) const noexcept {
    return !(*this == other);
  }

  Iterator begin() {  // NOLINT
    return VectorIterator(data_);
  }

  Iterator end() {  // NOLINT
    return VectorIterator<T>(data_ + size_);
  }

  ConstIterator begin() const {  // NOLINT
    return ConstVectorIterator<T>(data_);
  }

  ConstIterator end() const {  // NOLINT
    return ConstVectorIterator<T>(data_ + size_);
  }

  ConstIterator cbegin() const {  // NOLINT
    return ConstVectorIterator<T>(data_);
  }

  ConstIterator cend() const {  // NOLINT
    return ConstVectorIterator<T>(data_ + size_);
  }

  ReverseIterator rbegin() {  // NOLINT
    return std::reverse_iterator<Iterator>(data_ + size_);
  }

  ReverseIterator rend() {  // NOLINT
    return std::reverse_iterator<Iterator>(data_);
  }

  ConstReverseIterator rbegin() const {  // NOLINT
    return std::reverse_iterator<ConstIterator>(data_ + size_);
  }

  ConstReverseIterator rend() const {  // NOLINT
    return std::reverse_iterator<ConstIterator>(data_);
  }

  ConstReverseIterator crbegin() const {  // NOLINT
    return std::reverse_iterator<ConstIterator>(data_ + size_);
  }

  ConstReverseIterator crend() const {  // NOLINT
    return std::reverse_iterator<ConstIterator>(data_);
  }
};

#endif