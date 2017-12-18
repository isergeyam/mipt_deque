#include <iterator>

template <class T, int INC_COEF = 2, int DEC_COEF = 4>

class Deque {
private:
  size_t capacity_;
  T *array_;
  size_t head_;
  size_t tail_;
  void fix_alloc(bool incr) {
    size_t new_capacity = (incr) ? capacity_ << 1 : capacity_ >> 1;
    T *res = new T[new_capacity];
    size_t size_ = (incr) ? capacity_ - 1 : size();
    for (size_t i = 0; i < size_; ++i) {
      res[i] = array_[head_];
      head_ = (head_ + 1) % capacity_;
    }
    delete[] array_;
    array_ = res;
    head_ = 0;
    tail_ = size_;
    capacity_ = new_capacity;
    return;
  }
  void increase() {
    fix_alloc(true);
    return;
  }
  void decrease() {
    fix_alloc(false);
    return;
  }
  void fix_size() {
    if (head_ == (tail_ + 1) % capacity_)
      increase();
    if (size() < capacity_ / DEC_COEF)
      decrease();
    return;
  }

public:
  template <typename _Ref, typename _Ptr>
  struct _Deque_iterator : public std::iterator<std::random_access_iterator_tag,
                                                T, std::ptrdiff_t, T *, _Ref> {
    using base = std::iterator<std::random_access_iterator_tag, T,
                               std::ptrdiff_t, T *, _Ref>;
    using typename base::pointer;
    using typename base::reference;
    using typename base::difference_type;
    using typename base::value_type;
    typedef _Deque_iterator _Self;
    _Ptr start_;
    size_t cur_;
    explicit _Deque_iterator(_Ptr start_ = nullptr, size_t cur_ = 0)
        : start_(start_), cur_(cur_) {}
    explicit _Deque_iterator(const _Self &copy)
        : start_(copy.start_), cur_(copy.cur_) {}
    const _Self &operator=(const _Self &a) {
      cur_ = a.cur_;
      start_ = a.start_;
      return *this;
    }
    operator _Deque_iterator<const T &, const Deque<T> *>() const {
      return _Deque_iterator<const T &, const Deque<T> *>(
          const_cast<Deque<T> *>(start_), cur_);
    }
    bool operator==(const _Self &a) {
      return start_ == a.start_ && cur_ == a.cur_;
    }
    bool operator!=(const _Self &a) { return !(cur_ == a.cur_); }
    reference operator*() { return start_->array_[cur_]; }
    _Self &operator++() {
      cur_ = (cur_ + 1) % start_->capacity_;
      return *this;
    }
    _Self operator++(int) {
      _Self ans = *this;
      ++(*this);
      return ans;
    }
    _Self &operator--() {
      cur_ = (cur_ + start_->capacity_ - 1) % start_->capacity_;
      return *this;
    }
    _Self operator--(int) {
      _Self ans = *this;
      --(this);
      return ans;
    }
    bool operator<(const _Self &a) {
      return (start_->head_ < start_->tail_ && cur_ < a.cur_) ||
             (start_->head_ > start_->tail_ && cur_ > a.cur_);
    }
    bool operator>(const _Self &a) { return !((*this) < a) && (*this) != a; }
    bool operator>=(const _Self &a) { return (*this) > a || (*this) = a; }
    bool operator<=(const _Self &a) { return (*this) < a || (*this) = a; }
    _Self &operator+=(int rhs) {
      if (rhs == 0 || start_->empty())
        return *this;
      if (rhs > 0) {
        if (start_->head_ < start_->tail_ || cur_ < start_->tail_) {
          cur_ += rhs;
          return *this;
        } else
          cur_ = (rhs + cur_) % start_->capacity_;
      } else {
        if (start_->head_ < start_->tail_ || cur_ > start_->tail_) {
          cur_ += rhs;
          return *this;
        } else
          cur_ = (cur_ + rhs + start_->capacity_) % start_->capacity_;
      }
      return *this;
    }
    _Self &operator-=(int rhs) { return operator+=(-rhs); }
    friend _Self operator+(const _Self &a, int b) {
      _Self it = a;
      it += b;
      return it;
    }
    friend _Self operator+(int b, const _Self &a) {
      _Self it = a;
      it += b;
      return it;
    }
    friend _Self operator-(const _Self &a, int b) {
      _Self it = a;
      it -= b;
      return it;
    }
    difference_type operator-(const _Self &a) const {
      difference_type d1, d2;
      if (cur_ < start_->head_)
        d1 = start_->capacity_ - start_->head_ + cur_;
      else
        d1 = cur_ - start_->head_;
      if (a.cur_ < start_->head_)
        d2 = start_->capacity_ - start_->head_ + a.cur_;
      else
        d2 = a.cur_ - start_->head_;
      return d1 - d2;
    }
    T *operator->() { return &start_->array_[cur_]; }
    const T *operator->() const { return &start_->array_[cur_]; }
    reference operator[](int num) { return *(*this + num); }
  };
  typedef _Deque_iterator<T &, Deque<T> *> iterator;
  typedef _Deque_iterator<const T &, const Deque<T> *> const_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef Deque _Self;
  explicit Deque(size_t capacity = 10) : capacity_(capacity) {
    tail_ = head_ = 0;
    array_ = new T[capacity_];
  }
  void clear() { delete[] array_; }
  ~Deque() { clear(); }
  size_t size() const {
    if (tail_ < head_)
      return capacity_ - head_ + tail_;
    else
      return tail_ - head_;
  }
  bool empty() const { return tail_ == head_; }
  explicit Deque(const Deque &copy)
      : capacity_(copy.capacity_), array_(new T[capacity_]), head_(copy.head_),
        tail_(copy.tail_) {
    for (size_t i = head_; i != tail_; i = (i + 1) % capacity_)
      array_[i] = copy.array_[i];
  }
  template <typename InputIterator>
  explicit Deque(InputIterator begin, InputIterator end)
      : capacity_(INC_COEF * std::distance(begin, end)), head_(0),
        tail_(std::distance(begin, end)) {
    array_ = new T[capacity_];
    std::copy(begin, end, array_);
  }
  explicit Deque(Deque &&mv)
      : array_(mv.array_), head_(mv.head_), tail_(mv.tail_) {
    mv.array_ = nullptr;
    mv.head_ = mv.tail_ = 0;
  }
  _Self &operator=(const _Self &cp) {
    head_ = cp.head_;
    tail_ = cp.tail_;
    capacity_ = cp.capacity_;
    delete[] array_;
    array_ = new T[capacity_];
    for (size_t i = head_; i != tail_; i = (i + 1) % capacity_)
      array_[i] = cp.array_[i];
    return *this;
  }
  T &operator[](size_t num) {
    iterator it = begin();
    it += num;
    return *it;
  }
  T &at(size_t num) { return operator[](num); }
  const T &at(size_t num) const { return operator[](num); }
  const T &operator[](size_t num) const {
    const_iterator it = cbegin();
    it += num;
    return *it;
  }
  T &back() { return *rbegin(); }
  const T &back() const { return *crbegin(); }
  T &front() { return *begin(); }
  const T &front() const { return *cbegin(); }
  void push_back(const T &x) {
    fix_size();
    array_[tail_] = x;
    tail_ = (tail_ + 1) % capacity_;
    return;
  }
  void push_front(const T &x) {
    fix_size();
    head_ = (head_ - 1 + capacity_) % capacity_;
    array_[head_] = x;
    return;
  }
  void pop_back() {
    fix_size();
    if (empty())
      return;
    tail_ = (tail_ - 1 + capacity_) % capacity_;
    return;
  }
  void pop_front() {
    fix_size();
    if (empty())
      return;
    head_ = (head_ + 1) % capacity_;
    return;
  }
  _Self &operator=(_Self &&mv) {
    head_ = mv.head_;
    tail_ = mv.tail_;
    capacity_ = mv.capacity_;
    array_ = mv.array_;
    mv.array_ = nullptr;
    mv.head_ = mv.tail_ = 0;
    return *this;
  }
  iterator begin() { return iterator(this, head_); }
  const_iterator begin() const { return const_iterator(this, head_); }
  const_iterator cbegin() const { return const_iterator(this, head_); }
  iterator end() { return iterator(this, tail_); }
  const_iterator end() const { return const_iterator(this, tail_); }
  const_iterator cend() const { return const_iterator(this, tail_); }
  reverse_iterator rbegin() { return reverse_iterator(end()); }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(cend());
  }
  const_reverse_iterator crbegin() const {
    return const_reverse_iterator(cend());
  }
  reverse_iterator rend() { return reverse_iterator(begin()); }
  const_reverse_iterator rend() const {
    return const_reverse_iterator(cbegin());
  }
  const_reverse_iterator crend() const {
    return const_reverse_iterator(cbegin());
  }
};
