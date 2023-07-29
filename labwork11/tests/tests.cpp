#include "gtest/gtest.h"
#include "../lib/funcs.h"
#include "../lib/xrange.h"
#include "../lib/zip.h"
#include <array>
#include <deque>
#include <forward_list>
#include <list>
#include <set>
#include <string>
#include <vector>

TEST(STLAlgos, AllOfTest) {
    std::deque<int> d({123, 42, 11, 23});
    ASSERT_FALSE(my_funcs::all_of(d.begin(), d.end(), [](int a) { return a < 100; }));
    ASSERT_TRUE(my_funcs::all_of(d.begin(), d.end(), [](int a) { return a > 10; }));

    std::vector<std::string> v({"aml", "amber", "qwerty"});
    ASSERT_TRUE(my_funcs::all_of(v.begin(), v.end(), [](std::string& s) { return s.size() < 7; }));
    ASSERT_FALSE(my_funcs::all_of(v.begin(), v.end(), [](std::string& s) { return s.front() == 'a'; }));
}

TEST(STLAlgos, AnyOfTest) {
    std::forward_list<int> fl({77, 153, 1001, -5, 97531});
    ASSERT_FALSE(my_funcs::any_of(fl.begin(), fl.end(), [](int a) { return a % 2 == 0; }));
    ASSERT_TRUE(my_funcs::any_of(fl.begin(), fl.end(), [](int a) { return a < 0; }));
}

TEST(STLAlgos, NoneOfTest) {
    std::list<int> l({77, 153, 1001, 5, 97531});
    ASSERT_TRUE(my_funcs::none_of(l.begin(), l.end(), [](int a) { return a % 2 == 0; }));
    ASSERT_TRUE(my_funcs::none_of(l.begin(), l.end(), [](int a) { return a < 0; }));
    l.push_back(-2);
    ASSERT_FALSE(my_funcs::none_of(l.begin(), l.end(), [](int a) { return a % 2 == 0; }));
    ASSERT_FALSE(my_funcs::none_of(l.begin(), l.end(), [](int a) { return a < 0; }));
}

TEST(STLAlgos, OneOfTest) {
    std::set<int> s({5, 92, -999, 0, -13});
    ASSERT_TRUE(my_funcs::one_of(s.begin(), s.end(), [](int a) { return abs(a) < 3; }));
    ASSERT_FALSE(my_funcs::one_of(s.begin(), s.end(), [](int a) { return a < 0; }));
    ASSERT_FALSE(my_funcs::one_of(s.begin(), s.end(), [](int a) { return a > -10000; }));
}

TEST(STLAlgos, IsSortedTest) {
    std::list<int> l({9, 15, 8092, 12423});
    ASSERT_FALSE(my_funcs::is_sorted(l.begin(), l.end(), [](int a, int b) { return a > b; }));
    ASSERT_TRUE(my_funcs::is_sorted(l.begin(), l.end(), [](int a, int b) { return a < b; }));
    l.push_back(0);
    ASSERT_FALSE(my_funcs::is_sorted(l.begin(), l.end(), [](int a, int b) { return a < b; }));
}

TEST(STLAlgos, IsPartitionedTest) {
    std::forward_list<int> fl({1, 2, 3, 4, 6, 7, 8, 9});
    ASSERT_TRUE(my_funcs::is_partitioned(fl.begin(), fl.end(), [](int a) { return a < 5; }));
    ASSERT_TRUE(my_funcs::is_partitioned(fl.begin(), fl.end(), [](int a) { return a > 5; }));
    ASSERT_FALSE(my_funcs::is_partitioned(fl.begin(), fl.end(), [](int a) { return a > 0; }));
    ASSERT_FALSE(my_funcs::is_partitioned(fl.begin(), fl.end(), [](int a) { return a < 10; }));
}

TEST(STLAlgos, FindNotTest) {
    std::array<std::string, 8> a({"qwe", "qwe", "qwe", "qwe", "rty", "qwe", "ans", "qwe"});
    ASSERT_EQ(*my_funcs::find_not(a.begin(), a.end(), "qwe"), "rty");
}

TEST(STLAlgos, FindBackwards) {
    std::list<int> l({1, 3, 5, 3, 4});
    auto ans = my_funcs::find_backward(l.begin(), l.end(), 3);
    ASSERT_EQ(*ans, 3);
    ASSERT_EQ(*(++ans), 4);
}

TEST(STLAlgos, IsPalindromeTest) {
    std::deque<std::string> d({"3452", "lk", "sour"});
    ASSERT_TRUE(my_funcs::is_palindrome(d.begin(), d.end(), [](std::string& str) { return str.size(); }));
    d.emplace_back("q");
    ASSERT_FALSE(my_funcs::is_palindrome(d.begin(), d.end(), [](std::string& str) { return str.size(); }));
    d.emplace_front("1");
    ASSERT_TRUE(my_funcs::is_palindrome(d.begin(), d.end(), [](std::string& str) { return str.size(); }));
}

TEST(XRange, EndTest) {
    auto x = xrange(6);
    std::vector<int> v(x.begin(), x.end());
    ASSERT_EQ(v.size(), 6);
    for (int i = 0; i < v.size(); ++i) {
        ASSERT_EQ(v[i], i);
    }
    int cur = 0;
    for (auto i: xrange(2)) {
        ASSERT_EQ(i, cur);
        cur++;
    }
    cur = 0;
    for (auto i: xrange(-2)) {
        ASSERT_EQ(i, cur);
        cur--;
    }
}

const double start1 = 5.35;
const double start2 = -1.5;

TEST(XRange, BeginAndEndTest) {
    double cur = start1;
    for (auto i: xrange(start1, 10.15)) {
        ASSERT_EQ(i, cur);
        cur++;
    }
    cur = start2;
    for (auto i: xrange(-1.5, -16.423)) {
        ASSERT_EQ(i, cur);
        cur--;
    }
}

TEST(XRange, BeginEndAndStepTest) {
    double cur = start1;
    const double step1 = 1.43;
    for (auto i: xrange(start1, 10.15, step1)) {
        ASSERT_EQ(i, cur);
        cur += step1;
    }
    cur = start2;
    const int step2 = -3;
    for (auto i: xrange(start2, -16.423, step2)) {
        ASSERT_EQ(i, cur);
        cur += step2;
    }
}

TEST(XRange, StringTest) {
    std::string str = "text";
    auto x = xrange(str, std::string("textababababababab"), std::string("ab"));
    std::string ans = str;
    for (auto& s: x) {
        ASSERT_EQ(s, ans);
        ans += "ab";
    }
}

TEST(XRange, ErrorsTest) {
    ASSERT_THROW(xrange(2, 10, -2), std::invalid_argument);
    ASSERT_THROW(xrange(2, 10, 0), std::logic_error);
}

TEST(Zip, ZipTest) {
    std::list<int> l({1, 2, 3, 4, 5});
    std::vector<char> v({'a', 'b', 'c', 'd'});
    std::vector<std::pair<int, char>> ans({{1, 'a'},
                                           {2, 'b'},
                                           {3, 'c'},
                                           {4, 'd'}});
    int i = 0;
    for (auto value: zip(l, v)) {
        ASSERT_EQ(value, ans[i]);
        i++;
    }
}