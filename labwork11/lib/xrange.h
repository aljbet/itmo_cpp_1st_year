#include <iostream>

template<typename T, typename M>
class XRangeObject {
public:
    using difference_type = long long;
    using value_type = T;
    using reference = T&;
    using iterator_category = std::input_iterator_tag;

    XRangeObject() = default;

    XRangeObject(reference b, reference e, M& s) : begin_(b), end_(e), step_(s) {};

    XRangeObject& begin() {
        return *this;
    }

    XRangeObject& end() {
        return *this;
    }

    bool operator!=(XRangeObject& rv) {
        return begin_ < end_;
    }

    void operator++() {
        begin_ += step_;
    }

    reference operator*() {
        return begin_;
    }

private:
    value_type begin_;
    value_type end_;
    M step_;
};

template<typename T, typename M>
XRangeObject<T, M> xrange(T begin, T end, M step) {
    if (step == M()) {
        throw std::logic_error("step cannot be zero");
    }
    if ((begin > end && step > M()) || (begin < end && step < M())) {
        throw std::invalid_argument("end is unreachable with this step");
    }
    XRangeObject<T, M> x(begin, end, step);
    return x;
}

template<typename T>
XRangeObject<T, int> xrange(T begin, T end) {
    if (begin > end) {
        return xrange(begin, end, -1);
    }
    return xrange(begin, end, 1);
}

template<typename T>
XRangeObject<T, int> xrange(T end) {
    if (end < 0) {
        return xrange(0, end, -1);
    }
    return xrange(0, end, 1);
}