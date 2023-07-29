template<typename FirstType, typename SecondType>
class ZipObject {
public:
    using first_type = FirstType::value_type;
    using second_type = SecondType::value_type;
    using first_iterator = FirstType::iterator;
    using second_iterator = SecondType::iterator;
    using value_type = std::pair<first_type, second_type>;

    ZipObject() = default;

    ZipObject(FirstType& b, SecondType& e) : it1(b.begin()), it2(e.begin()), end_(std::min(b.size(), e.size())) {};

    ZipObject& begin() {
        return *this;
    }

    ZipObject& end() {
        return *this;
    }

    bool operator!=(ZipObject& rv) {
        return begin_ < end_;
    }

    void operator++() {
        begin_++;
        it1++;
        it2++;
    }

    value_type operator*() {
        return std::make_pair(*it1, *it2);
    }

private:
    first_iterator it1;
    second_iterator it2;
    int begin_ = 0;
    int end_;
};

template<typename F, typename S>
ZipObject<F, S> zip(F first, S second) {
    ZipObject<F, S> z(first, second);
    return z;
}

