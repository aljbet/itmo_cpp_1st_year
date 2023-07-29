#pragma once

#include "CIterator.h"
#include <initializer_list>

template<typename T, class A = std::allocator<T>>
class CCircularBuffer {
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using const_reference = const T&;
    using iterator = CIterator<T>;
    using allocator = A;
    using const_iterator = CConstIterator<T>;
    using size_type = size_t;

protected:
    size_type capacity_;
    size_type size_;
    allocator alloc_ = A();
    pointer alloc_pointer_;
    iterator buffer_begin_ = nullptr;
    iterator buffer_end_ = nullptr;
    iterator data_end_ = nullptr;

public:
    explicit CCircularBuffer() : capacity_(1), size_(0), alloc_pointer_(alloc_.allocate(capacity_)),
                                 buffer_begin_(alloc_pointer_), buffer_end_(alloc_pointer_ + capacity_),
                                 data_end_(alloc_pointer_) {}

    explicit CCircularBuffer(const size_type& size) : capacity_(size), size_(0),
                                                      alloc_pointer_(alloc_.allocate(capacity_)),
                                                      buffer_begin_(alloc_pointer_),
                                                      buffer_end_(alloc_pointer_ + capacity_),
                                                      data_end_(alloc_pointer_) {
        for (size_type i = 0; i < capacity_; i++) {
            std::allocator_traits<A>::construct(alloc_, alloc_pointer_ + i, value_type());
        }
    }

    explicit CCircularBuffer(const size_type& size, const_reference value) : capacity_(size), size_(size),
                                                                             alloc_pointer_(alloc_.allocate(capacity_)),
                                                                             buffer_begin_(alloc_pointer_),
                                                                             buffer_end_(alloc_pointer_ + capacity_),
                                                                             data_end_(alloc_pointer_ + capacity_) {
        for (size_type i = 0; i < capacity_; i++) {
            std::allocator_traits<A>::construct(alloc_, alloc_pointer_ + i, value_type());
        }
        std::fill(buffer_begin_.get_pointer(), buffer_end_.get_pointer(), value);
    }

    explicit CCircularBuffer(pointer i, pointer j) : capacity_(j - i), size_(j - i),
                                                     alloc_pointer_(alloc_.allocate(capacity_)),
                                                     buffer_begin_(alloc_pointer_),
                                                     buffer_end_(alloc_pointer_ + capacity_),
                                                     data_end_(alloc_pointer_ + capacity_) {
        for (size_type k = 0; k < capacity_; k++) {
            std::allocator_traits<A>::construct(alloc_, alloc_pointer_ + k, value_type());
        }
        std::copy(i, j, buffer_begin_.get_pointer());
    }

    explicit CCircularBuffer(iterator i, iterator j) : capacity_(j - i), size_(j - i),
                                                       alloc_pointer_(alloc_.allocate(capacity_)),
                                                       buffer_begin_(alloc_pointer_),
                                                       buffer_end_(alloc_pointer_ + capacity_),
                                                       data_end_(alloc_pointer_ + capacity_) {
        for (size_type k = 0; k < capacity_; k++) {
            std::allocator_traits<A>::construct(alloc_, alloc_pointer_ + k, value_type());
        }
        std::copy(i, j, buffer_begin_.get_pointer());
    }

    CCircularBuffer(const std::initializer_list<value_type>& il) : capacity_(il.end() - il.begin()),
                                                                   size_(il.end() - il.begin()),
                                                                   alloc_pointer_(alloc_.allocate(capacity_)),
                                                                   buffer_begin_(alloc_pointer_),
                                                                   buffer_end_(alloc_pointer_ + capacity_),
                                                                   data_end_(alloc_pointer_ + capacity_) {
        for (size_type i = 0; i < capacity_; i++) {
            std::allocator_traits<A>::construct(alloc_, alloc_pointer_ + i, value_type());
        }
        std::copy(il.begin(), il.end(), buffer_begin_.get_pointer());
    }

    ~CCircularBuffer() {
        clear();
    }

    [[nodiscard]] bool empty() const {
        return size_ == 0;
    }

    [[nodiscard]] size_type size() const {
        return size_;
    }

    [[nodiscard]] size_type capacity() const {
        return capacity_;
    }

    iterator begin() const {
        return buffer_begin_;
    }

    iterator end() const {
        return data_end_;
    }

    const_iterator cbegin() const {
        return const_iterator(const_cast<pointer>(buffer_begin_.get_pointer()));
    }

    const_iterator cend() const {
        return const_iterator(const_cast<pointer>(data_end_.get_pointer()));
    }

    value_type front() const {
        return *(buffer_begin_.get_pointer());
    }

    value_type back() const {
        return *(data_end_ - 1);
    }

    CCircularBuffer& operator=(const CCircularBuffer& rv) {
        std::copy(rv.begin(), rv.end(), buffer_begin_.get_pointer());
        capacity_ = rv.capacity_;
        size_ = rv.size_;
        data_end_ = buffer_begin_ + size_;
        buffer_end_ = buffer_begin_ + capacity_;
        return *this;
    }

    CCircularBuffer& operator=(const std::initializer_list<value_type>& il) {
        std::copy(il.begin(), il.end(), buffer_begin_.get_pointer());
        capacity_ = il.size();
        size_ = il.size();
        data_end_ = buffer_begin_ + size_;
        buffer_end_ = buffer_begin_ + capacity_;
        return *this;
    }

    bool operator==(const CCircularBuffer& rv) const {
        bool ans = (capacity_ == rv.capacity_) && (size_ == rv.size_);
        if (!ans) {
            return ans;
        }
        for (int i = 0; i < size_; i++) {
            ans = *(buffer_begin_ + i) == *(rv.buffer_begin_ + i);
            if (!ans) {
                return ans;
            }
        }
        return ans;
    }

    bool operator!=(const CCircularBuffer& rv) const {
        return !(*this == rv);
    }

    void swap(CCircularBuffer& rv) {
        std::swap(capacity_, rv.capacity_);
        std::swap(size_, rv.size_);
        std::swap(buffer_begin_, rv.buffer_begin_);
        std::swap(data_end_, rv.data_end_);
        std::swap(buffer_end_, rv.buffer_end_);
    }

    reference operator[](size_type n) const {
        return *(buffer_begin_ + n % capacity_);
    }

    reference at(size_type n) const {
        return *(buffer_begin_ + n % capacity_);
    }

    virtual void insert(iterator p, const_reference t) {
        if (buffer_end_ == data_end_) {
            iterator temp;
            if (p != buffer_begin_) {
                temp = p - 1;
                while (temp != buffer_begin_) {
                    *temp = *(temp - 1);
                    --temp;
                }
                *buffer_begin_ = *(buffer_end_ - 1);
            }
            temp = buffer_end_ - 1;
            while (temp != p) {
                *temp = *(temp - 1);
                --temp;
            }
            *p = t;
        } else if (size_ == 0) {
            std::allocator_traits<A>::construct(alloc_, data_end_.get_pointer(), value_type());
            *buffer_begin_ = t;
            size_++;
            data_end_++;
        } else {
            iterator temp = data_end_;
            std::allocator_traits<A>::construct(alloc_, data_end_.get_pointer(), value_type());
            while (temp != p) {
                *temp = *(temp - 1);
                --temp;
            }
            *p = t;
            data_end_++;
            size_++;
        }
    }

    virtual void insert(iterator p, int n, const_reference t) {
        for (int i = 0; i < n; ++i) {
            insert(p, t);
        }
    }

    template<typename insert_iterator>
    void insert(iterator p, insert_iterator i, insert_iterator j) {
        j--;
        while (j >= i) {
            insert(p, *j);
            j--;
        }
    }

    virtual void insert(iterator p, std::initializer_list<value_type> il) {
        insert(p, il.begin(), il.end());
    }

    virtual void erase(iterator q) {
        while (q != data_end_ - 1) {
            *q = *(q + 1);
            q++;
        }
        size_--;
        data_end_--;
    }

    void erase(iterator q1, iterator q2) {
        size_type r = q2 - q1;
        while (q1 + r < data_end_) {
            *q1 = *(q1 + r);
            q1++;
        }
        size_ -= r;
        data_end_ -= r;
    }

    void clear() {
        for (iterator i = buffer_begin_; i < buffer_end_; i++) {
            std::allocator_traits<A>::destroy(alloc_, i.get_pointer());
        }
        alloc_.deallocate(buffer_begin_.get_pointer(), capacity_);
        alloc_pointer_ = alloc_.allocate(capacity_);
        buffer_begin_ = alloc_pointer_;
        buffer_end_ = alloc_pointer_ + capacity_;
        data_end_ = alloc_pointer_;
        size_ = 0;
    }

    void assign(pointer i, pointer j) {
        clear();
        CCircularBuffer<value_type, allocator> new_buff(i, j);
        *this = new_buff;
    }

    void assign(const std::initializer_list<value_type>& il) {
        if (!empty()) {
            clear();
        }
        *this = il;
    }

    void assign(const size_type& n, const_reference t) {
        clear();
        CCircularBuffer<value_type, allocator> new_buff(n, t);
        *this = new_buff;
    }

    void resize(const size_type& n) {
        if (n <= capacity_) {
            size_ = std::min(n, size_);
            data_end_ = buffer_begin_ + size_;
        } else {
            CCircularBuffer<value_type, A> new_buff(n);
            new_buff.size_ = n;
            new_buff.data_end_ = new_buff.buffer_end_;
            std::fill(new_buff.buffer_begin_.get_pointer(), new_buff.buffer_end_.get_pointer(), value_type());
            for (int i = 0; i < size_; i++) {
                new_buff[i] = (*this)[i];
            }
            *this = new_buff;
        }
    }

    void reserve(const size_type& n) {
        if (n > capacity()) {
            CCircularBuffer<value_type, A> new_buff(n);
            for (int i = 0; i < size_; i++) {
                new_buff[i] = (*this)[i];
                new_buff.size_++;
                new_buff.data_end_++;
            }
            *this = new_buff;
        }
    }
};

template<typename T, class A = std::allocator<T>>
class CExtCircularBuffer final : public CCircularBuffer<T, A> {
    using value_type = T;
    using pointer = T*;
    using const_reference = const T&;
    using iterator = CIterator<T>;
    using size_type = unsigned int;
    using parent_buffer = CCircularBuffer<T, A>;

public:
    explicit CExtCircularBuffer() : parent_buffer() {}

    explicit CExtCircularBuffer(const size_type& size) : parent_buffer(size) {}

    explicit CExtCircularBuffer(const size_type& size, const_reference value) : parent_buffer(size, value) {}

    explicit CExtCircularBuffer(pointer i, pointer j) : parent_buffer(i, j) {}

    explicit CExtCircularBuffer(iterator i, iterator j) : parent_buffer(i, j) {}

    CExtCircularBuffer(const std::initializer_list<value_type>& il) : parent_buffer(il) {}

    ~CExtCircularBuffer() {
        this->clear();
    }

    void insert(iterator p, const_reference t) override {
        if (this->buffer_end_ == this->data_end_) {
            this->reserve(this->capacity() * 2);
            iterator temp = this->data_end_;
            std::allocator_traits<A>::construct(this->alloc_, this->data_end_.get_pointer(), value_type());
            while (temp != p) {
                *temp = *(temp - 1);
                --temp;
            }
            *p = t;
            this->data_end_++;
            this->size_++;
        } else if (this->size_ == 0) {
            std::allocator_traits<A>::construct(this->alloc_, this->data_end_.get_pointer(), value_type());
            *this->buffer_begin_ = t;
            this->size_++;
            this->data_end_++;
        } else {
            iterator temp = this->data_end_;
            std::allocator_traits<A>::construct(this->alloc_, this->data_end_.get_pointer(), value_type());
            while (temp != p) {
                *temp = *(temp - 1);
                --temp;
            }
            *p = t;
            this->data_end_++;
            this->size_++;
        }
    }

    void insert(iterator p, int n, const_reference t) override {
        for (int i = 0; i < n; ++i) {
            insert(p, t);
        }
    }

    template<typename insert_iterator>
    void insert(iterator p, insert_iterator i, insert_iterator j) {
        j--;
        while (j >= i) {
            insert(p, *j);
            j--;
        }
    }

    void insert(iterator p, std::initializer_list<value_type> il) override {
        insert(p, il.begin(), il.end());
    }
};