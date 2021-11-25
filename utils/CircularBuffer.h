#ifndef __CIRCULAR_BUFFER_H__
#define __CIRCULAR_BUFFER_H__

#include <iterator>
#include <memory>
#include <compare>

template<class T, class A = std::allocator<T> >
class CircularBuffer {
public:
  // member types
  typedef CircularBuffer<T, A> self_type;
  typedef A allocator_type;
  typedef std::allocator_traits<allocator_type> traits_type;
  typedef traits_type::value_type value_type;
  typedef traits_type::pointer pointer;
  typedef traits_type::const_pointer const_pointer;
  typedef T& reference;
  typedef const T& const_reference;
  typedef traits_type::size_type size_type;
  typedef traits_type::difference_type difference_type;

  // constructor
  explicit CircularBuffer(size_type const capacity = 1,
                          allocator_type const &allocator = allocator_type())
    : capacity_(capacity), allocator_(allocator),
      array_(allocator_.allocate(capacity_)) {}

  // copy constructor
  CircularBuffer(self_type const &that)
    : capacity_(that.capacity_), allocator_(that.allocator_),
      array_(allocator_.allocate(capacity_)) {
    for (auto it = that.begin(); it != that.end(); ++it)
      push_back(*it);
  }

  // assignment operator
  CircularBuffer &operator=(self_type that) {
    std::swap(capacity_, that.capacity_);
    std::swap(array_, that.array_);
    std::swap(head_, that.head_);
    std::swap(tail_, that.tail_);
    std::swap(contents_size_, that.contents_size_);
  }
  
  // destrucutor
  ~CircularBuffer() {
    clear();
    allocator_.deallocate(array_, capacity_);
  }

  // iterator
  template<class E, class EN>
  class iterator_ : public std::iterator<std::random_access_iterator_tag, EN> {
  public:
    typedef E elem_type;
    typedef EN elem_type_nc;
    typedef iterator_<elem_type, elem_type_nc> self_type;

    iterator_(const CircularBuffer<T, A> *buf, size_type const pos)
      : buf_(buf), pos_(pos) {}

    self_type &operator=(self_type const &that) {
      buf_ = that.buf_;
      pos_ = that.pos_;
      return *this;
    }
    
    bool operator==(self_type const &that) const {
      return buf_ == that.buf_ && pos_ == that.pos_;
    }

    self_type &operator++() {
      pos_++;
      return *this;
    }
      
    self_type operator++(int) {
      self_type tmp(*this);
      ++(*this);
      return tmp;
    }

    self_type &operator--() {
      pos_--;
      return *this;
    }
      
    self_type operator--(int) {
      self_type tmp(*this);
      --(*this);
      return tmp;
    }

    difference_type operator-(self_type const &that) const {
      return pos_ - that.pos_;
    }
    
    // elem_type &operator*() {
    //   return (*buf_)[pos_];
    // }

    const value_type &operator*() const {
      return (*buf_)[pos_];
    }
    
    // elem_type *operator->() {
    //   return &(operator*());
    // }

    const value_type *operator->() const {
      return &(operator*());
    }

    // const value_type &operator[](int n) const {
    //   return *(*this + n);
    // }
    
  private:
    const CircularBuffer<T, A> *buf_;
    size_type pos_;
  };                            // end iterator

  // iterator member types
  typedef iterator_<value_type, value_type> iterator;
  typedef iterator_<const value_type, value_type> const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  // iterator-based template constructor
  CircularBuffer(iterator begin, iterator end) {
    capacity_ = std::distance(begin, end);
    array_ = allocator_.allocate(capacity_);
    while (begin != end)
      push_back(*begin++);
  }

  // iterators
  iterator begin() { return iterator(this, 0); }
  iterator end() { return iterator(this, contents_size_); }
  const_iterator begin() const { return const_iterator(this, 0); }
  const_iterator end() const { return const_iterator(this, contents_size_); }
    
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

  // capacity
  bool empty() const { return contents_size_ == 0; }
  size_type capacity() const { return capacity_; }
  size_type size() const { return contents_size_; }
  size_type max_size() const { return traits_type::max_size(allocator_); }

  bool full() const { return contents_size_ == capacity_; }

  // modifiers
  template<class TT>
  void push_back(TT &&item) {
    size_type next = (tail_ + 1 == capacity_) ? 0 : tail_ + 1;
    if (contents_size_ == capacity_) [[unlikely]] {
      array_[next] = std::forward<TT>(item);
      increment_head();
    }
    else [[likely]] {
      traits_type::construct(allocator_, array_ + next, std::forward<TT>(item));
    }
    increment_tail();
  }

  void pop_front() {
    size_type pos = head_;
    increment_head();
    traits_type::destroy(allocator_, array_ + pos);
  }

  void clear() {
    for (size_type n = 0; n < contents_size_; ++n)
      traits_type::destroy(allocator_, array_ + (n + head_) % capacity_);
    head_ = 1;
    tail_ = contents_size_ = 0;
  }

  // element access
  reference front() { return array_[head_]; }
  reference back() { return array_[tail_]; }
  const_reference front() const { return array_[head_]; }
  const_reference back() const { return array_[tail_]; }

  reference operator[](size_type const n) {
    return array_[(n + head_) % capacity_];
  }
  const_reference operator[](size_type const n) const {
    return array_[(n + head_) % capacity_];
  }
  
  
private:
  void increment_head() {
    --contents_size_;
    [[unlikely]] head_ = (head_ + 1 == capacity_) ? 0 : head_ + 1;
  }

  void increment_tail() {
    ++contents_size_;
    [[unlikely]] tail_ = (tail_ + 1 == capacity_) ? 0 : tail_ + 1;
  }
  
  size_type capacity_;
  allocator_type allocator_;
  pointer array_;
  size_type head_ = 1;
  size_type tail_ = 0;
  size_type contents_size_ = 0;
};

#endif  // __CIRCULAR_BUFFER_H__
