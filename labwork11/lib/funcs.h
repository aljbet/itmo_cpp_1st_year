#include <iostream>

namespace my_funcs {
    template<typename iterator, typename predicate>
    constexpr bool all_of(iterator first, iterator last, predicate pred) {
        iterator it = first;
        while (it != last) {
            if (!pred(*it)) {
                return false;
            }
            it++;
        }
        return true;
    }

    template<typename iterator, typename predicate>
    constexpr bool any_of(iterator first, iterator last, predicate pred) {
        iterator it = first;
        while (it != last) {
            if (pred(*it)) {
                return true;
            }
            it++;
        }
        return false;
    }

    template<typename iterator, typename predicate>
    constexpr bool none_of(iterator first, iterator last, predicate pred) {
        iterator it = first;
        while (it != last) {
            if (pred(*it)) {
                return false;
            }
            it++;
        }
        return true;
    }

    template<typename iterator, typename predicate>
    constexpr bool one_of(iterator first, iterator last, predicate pred) {
        iterator it = first;
        bool found_element = false;
        while (it != last) {
            if (pred(*it)) {
                if (found_element) return false;
                else found_element = true;
            }
            it++;
        }
        return found_element;
    }

    template<typename iterator, typename predicate>
    constexpr bool is_sorted(iterator first, iterator last, predicate pred) {
        iterator current_it = first;
        iterator next_it = ++first;
        while (next_it != last) {
            if (!pred(*current_it, *next_it)) {
                return false;
            }
            current_it = next_it;
            next_it++;
        }
        return true;
    }

    template<typename iterator, typename predicate>
    constexpr bool is_partitioned(iterator first, iterator last, predicate pred) {
        bool first_pred = pred(*first);
        bool partitioner_found = false;
        iterator it = first;
        while (it != last) {
            if (first_pred != pred(*it) && !partitioner_found) {
                partitioner_found = true;
                first_pred = !first_pred;
            } else if (first_pred != pred(*it) && partitioner_found) {
                return false;
            }
            it++;
        }
        return first_pred != pred(*first);
    }

    template<typename iterator, typename T>
    constexpr iterator find_not(iterator first, iterator last, T a) {
        iterator it = first;
        while (it != last) {
            if (*it != a) {
                return it;
            }
            it++;
        }
        return it;
    }

    template<typename iterator, typename T>
    constexpr iterator find_backward(iterator first, iterator last, T a) {
        iterator it = first;
        iterator ans;
        while (it != last) {
            if (*it == a) {
                ans = it;
            }
            it++;
        }
        return ans;
    }

    template<typename iterator, typename predicate>
    constexpr bool is_palindrome(iterator first, iterator last, predicate pred) {
        iterator begin_it = first;
        iterator end_it = last - 1;
        while (begin_it < end_it) {
            if (pred(*begin_it) != pred(*end_it)) {
                return false;
            }
            begin_it++;
            end_it--;
        }
        return true;
    }
}