#include <iterator>
template<class T, typename _Ref, typename _Ptr>
struct _Deque_iterator ;
template<class T>
class Deque{
	private:
		size_t capacity_ ;
		T *array_ ;
		size_t head_ ;
		size_t tail_ ;
		void increase() {
			T *res=new T[capacity_<<1] ;
			for (size_t i=0;i<capacity_-2;++i) {
				res[i]=array_[head_] ;
				head_=(head_+1)%capacity_ ;
			}
			delete[] array_ ;
			array_=res ;
			head_=0 ;
			tail_=capacity_-1 ;
			capacity_<<=1 ;
			return ;
		}
		void decrease() {
			T *res=new T[capacity_>>1] ;
			size_t size_=size() ;
			for (size_t i=0;i<size_-1;++i) {
				res[i]=array_[head_] ;
				head_=(head_+1)%capacity_ ;
			}
			delete[] array_ ;
			array_=res ;
			head_=0 ;
			tail_=size_ ;
			capacity_>>=1 ;
			return ;
		}
	public:
		template<class, typename, typename> friend struct _Deque_iterator ;
		typedef _Deque_iterator<T, T&, Deque<T>*> iterator ;
		typedef _Deque_iterator<T, const T&, const Deque<T>*> const_iterator ;
		typedef std::reverse_iterator<const_iterator>      const_reverse_iterator;
		typedef std::reverse_iterator<iterator>            reverse_iterator;
		Deque(size_t capacity=10) : capacity_(capacity) {
			tail_=head_=0 ;
			array_=new T[capacity_] ;
		}
		~Deque() {
			delete[] array_ ;
		}
		size_t size() const {
			if (tail_<head_)
				return capacity_-head_+tail_ ;
			else
				return tail_-head_ ;
		}
		bool empty() const {
			return tail_==head_ ;
		}
		Deque(const Deque &copy, size_t capacity=0) :
			capacity_(capacity),
			head_(copy.head_),
			tail_(copy.tail_) {
				if (capacity==0)
					capacity_=copy.capacity_ ;
				array_=new T[capacity_] ;
				for (size_t i=head_;i!=tail_;i=(i+1)%copy.size())
					array_[i]=copy.array_[i] ;
			}
		T& operator[] (size_t num) {
			iterator it(this, head_) ;
			it+=num ;
			return *it ;
		}
		const T& operator[](size_t num) const {
			const_iterator it(this, head_) ;
			it+=num ;
			return *it ;
		}
		T& back() {
			return array_[(tail_-1+capacity_)%capacity_] ;
		}
		const T& back() const {
			return array_[(tail_-1+capacity_)%capacity_] ;
		}
		T& front() {
			return array_[head_] ;
		}
		const T& front() const {
			return array_[head_] ;
		}
		void push_back(const T &x) {
			if (head_==(tail_+1)%capacity_)
				increase() ;
			array_[tail_]=x ;
			tail_=(tail_+1)%capacity_ ;
			return ;
		}
		void push_front(const T &x) {
			if (head_==(tail_+1)%capacity_)
				increase() ;
			head_=(head_-1+capacity_)%capacity_ ;
			array_[head_]=x ;
			return ;
		}
		void pop_back() {
			if (size()<capacity_/4)
				decrease() ;
			tail_=(tail_-1+capacity_)%capacity_ ;
			return ;
		}
		void pop_front() {
			if (size()<capacity_/4)
				decrease() ;
			head_=(head_+1)%capacity_ ;
			return ;
		}
		iterator begin() {
			return iterator(this, head_) ;
		}
		const_iterator begin() const {
			return const_iterator(this, head_) ;
		}
		const_iterator cbegin() const {
			return const_iterator(this, head_) ;
		}
		iterator end() {
			return iterator(this, tail_) ;
		}
		const_iterator end() const {
			return const_iterator(this, tail_) ;
		}
		const_iterator cend() const {
			return const_iterator(this, tail_) ;
		}
		reverse_iterator rbegin() {
			iterator it=end() ;
			--it ;
			return reverse_iterator(it) ;
		}
		const_reverse_iterator rbegin() const {
			const_iterator it=cend() ;
			--it ;
			return const_reverse_iterator(it) ;
		}
		const_reverse_iterator crbegin() const {
			const_iterator it=cend() ;
			--it ;
			return const_reverse_iterator(it) ;
		}
		reverse_iterator rend() {
			iterator it=begin() ;
			return reverse_iterator(it) ;
		}
		const_reverse_iterator rend() const {
			const_iterator it=cbegin() ;
			return const_reverse_iterator(it) ;
		}
		const_reverse_iterator crend() const {
			const_iterator it=cbegin() ;
			return const_reverse_iterator(it) ;
		}
};
template<class T, typename _Ref, typename _Ptr>
struct _Deque_iterator : public std::iterator<
												std::random_access_iterator_tag,
												T,
												std::ptrdiff_t,
												T*,
												_Ref>
{
	using base= std::iterator<
													std::random_access_iterator_tag,
													T,
													std::ptrdiff_t,
													T*,
													_Ref> ;
	using typename base::pointer ;
	using typename base::reference ;
	using typename base::difference_type ;
	using typename base::value_type ;
	typedef _Deque_iterator _Self ;
	_Ptr start_ ;
	size_t cur_ ;
	_Deque_iterator(Deque<T> *start_=nullptr, size_t cur_=0) :
		 start_(start_), cur_(cur_) {}
	_Deque_iterator(const _Self& copy) :
		start_(copy.start_), cur_(copy.cur_) {}
	const _Self& operator=(const _Self &a) {
		cur_=a.cur_ ;
		start_=a.start_ ;
		return *this ;
	}
	bool operator==(const _Self &a) {
		return start_==a.start_ && cur_==a.cur_ ;
	}
	bool operator!=(const _Self &a) {
		return !(cur_==a.cur_) ;
	}
	reference operator*() {
		return start_->array_[cur_] ;
	}
	_Self& operator++() {
		cur_=(cur_+1)%start_->capacity_ ;
		return *this ;
	}
	_Self operator++(int) {
		_Self ans = *this ;
		++(*this) ;
		return ans ;
	}
	_Self& operator--() {
		cur_=(cur_+start_->capacity_-1)%start_->capacity_ ;
		return *this ;
	}
	_Self operator--(int) {
		_Self ans = *this ;
		--(this) ;
		return ans ;
	}
	bool operator<(const _Self &a) {
		return (start_->head_<start_->tail_ && cur_<a.cur_) || (start_->head_>start_->tail_ && cur_>a.cur_) ;
	}
	bool operator>(const _Self &a) {
		return !((*this)<a) && (*this)!=a ;
	}
	bool operator>=(const _Self &a) {
		return (*this)>a || (*this)=a ;
	}
	bool operator<=(const _Self &a) {
		return (*this)<a || (*this)=a ;
	}
	_Self& operator+=(int rhs) {
		if (rhs==0 || start_->empty())
			return *this ;
		if (rhs>0) {
			if (start_->head_<start_->tail_ || cur_<start_->tail_) {
				cur_+=rhs ;
				return *this ;
			}
			else
				cur_=(rhs + cur_) % start_->capacity_ ;
		}
		else {
			if (start_->head_<start_->tail_ || cur_>start_->tail_) {
				cur_+=rhs ;
				return *this ;
			}
			else
				cur_=(cur_ + rhs + start_->capacity_) % start_->capacity_ ;
		}
		return *this ;
	}
	_Self& operator-=(int rhs) {
		return operator+=(-rhs) ;
	}
	friend _Self operator+(const _Self &a, int b) {
		_Self it=a ;
		it+=b ;
		return it ;
	}
	friend _Self operator+(int b, const _Self &a) {
		_Self it=a ;
		it+=b ;
		return it ;
	}
	friend _Self operator-(const _Self &a, int b) {
		_Self it=a ;
		it-=b ;
		return it ;
	}
};
