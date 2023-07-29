#ifndef LABWORK8_CITERATOR_H
#define LABWORK8_CITERATOR_H

#endif //LABWORK8_CITERATOR_H

#pragma once

#include <iostream>

template<typename T>
class CIterator {
    using pointer = T*;
    using reference = T&;
    using const_reference = const T&;
    using difference_type = ptrdiff_t;

private:
    pointer pointer_ = nullptr;

public:
    CIterator() = default;

    explicit CIterator(pointer value) : pointer_(value) {}

    ~CIterator() = default;

    pointer get_pointer() const {
        return pointer_;
    }

    bool operator==(const CIterator& it) const {
        return pointer_ == it.pointer_;
    }

    bool operator!=(const CIterator& it) const {
        return pointer_ != it.pointer_;
    }

    CIterator& operator++() {
        pointer_++;
        return *this;
    }

    CIterator operator++(int) {
        CIterator a = *this;
        ++(*this);
        return a;
    }

    CIterator& operator--() {
        pointer_--;
        return *this;
    }

    CIterator operator--(int) {
        CIterator a = *this;
        --(*this);
        return a;
    }

    reference operator*() {
        return *pointer_;
    }

    pointer operator->() const {
        return pointer_;
    }

    CIterator& operator=(reference rvalue) {
        *pointer_ = rvalue;
        return *this;
    }

    CIterator& operator=(pointer rvalue) {
        pointer_ = rvalue;
        return *this;
    }

    CIterator& operator+=(int n) {
        if (n >= 0) {
            while (n--) {
                ++(*this);
            }
        } else {
            while (n++) {
                --(*this);
            }
        }
        return *this;
    }

    CIterator& operator-=(int n) {
        return *this += (-n);
    }

    CIterator operator+(int n) const {
        CIterator temp = *this;
        temp += n;
        return temp;
    }

    CIterator operator-(int n) const {
        CIterator temp = *this;
        temp -= n;
        return temp;
    }

    difference_type operator-(CIterator rv) const {
        return pointer_ - rv.pointer_;
    }

    bool operator<(CIterator rv) const {
        return rv - *this > 0;
    }

    bool operator>(CIterator rv) const {
        return rv < *this;
    }

    bool operator<=(CIterator rv) const {
        return *this < rv || *this == rv;
    }

    bool operator>=(CIterator rv) const {
        return *this > rv || *this == rv;
    }
};

template<typename T>
class CConstIterator {
    using pointer = T*;
    using const_pointer = T const*;
    using reference = T&;
    using const_reference = T const&;
    using difference_type = ptrdiff_t;

private:
    const_pointer pointer_;

public:
    CConstIterator() = default;

    explicit CConstIterator(pointer value) : pointer_(value) {}

    ~CConstIterator() = default;

    bool operator==(const CConstIterator& it) const {
        return pointer_ == it.pointer_;
    }

    bool operator!=(const CConstIterator& it) const {
        return pointer_ != it.pointer_;
    }

    const_reference operator*() const {
        return *pointer_;
    }

    const_pointer operator->() const {
        return pointer_;
    }

    CConstIterator operator+(int n) const {
        CConstIterator temp = *this;
        temp += n;
        return temp;
    }

    CConstIterator operator-(int n) const {
        CConstIterator temp = *this;
        temp -= n;
        return temp;
    }

    difference_type operator-(CConstIterator rv) const {
        return pointer_ - rv.pointer_;
    }

    bool operator<(CConstIterator rv) const {
        return rv - *this > 0;
    }

    bool operator>(CConstIterator rv) const {
        return rv < *this;
    }

    bool operator<=(CConstIterator rv) const {
        return *this < rv || *this == rv;
    }

    bool operator>=(CConstIterator rv) const {
        return *this > rv || *this == rv;
    }
};