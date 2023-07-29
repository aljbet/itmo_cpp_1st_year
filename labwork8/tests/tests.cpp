#include <lib/CCircularBuffer.h>
#include <gtest/gtest.h>
#include <vector>
#include <string>

TEST(IteratorTest, EqualityTest) {
    CIterator<int> a;
    CIterator<int> b;
    int c = 5;
    a = &c;
    b = &c;
    ASSERT_EQ(a, b);
}

TEST(BufferTest, ConstructorSizeAndValue) {
    CCircularBuffer<std::string> buff(3, "aa");
    ASSERT_EQ(buff[0], "aa");
    ASSERT_EQ(buff[1], "aa");
    ASSERT_EQ(buff[2], "aa");
    ASSERT_EQ(buff.size(), 3);
    ASSERT_FALSE(buff.empty());
}

TEST(BufferTest, ConstructorTwoPointers) {
    std::vector<int> vec({2, 3, 9, 2015});
    CCircularBuffer<int> buff(vec.begin().base(), vec.end().base());
    ASSERT_EQ(buff[0], 2);
    ASSERT_EQ(buff.at(1), 3);
    ASSERT_EQ(buff[2], 9);
    ASSERT_EQ(buff[3], 2015);
    ASSERT_EQ(buff.size(), 4);
    ASSERT_EQ(buff.capacity(), 4);
    ASSERT_FALSE(buff.empty());
}

TEST(BufferTest, ConstructorFromInitializerList) {
    CCircularBuffer<int> buff({2, 3, 9, 2015});
    ASSERT_EQ(buff[0], 2);
    ASSERT_EQ(buff[1], 3);
    ASSERT_EQ(buff[2], 9);
    ASSERT_EQ(buff[3], 2015);
    ASSERT_EQ(buff.size(), 4);
    ASSERT_EQ(buff.capacity(), 4);
}

TEST(BufferTest, ConstructorFromAnotherBuffer) {
    CCircularBuffer<int> parent_buff({0, 1, 2, 3, 4, 5, 6});
    CCircularBuffer<int> buff(parent_buff.begin() + 2, parent_buff.begin() + 4);
    ASSERT_EQ(buff[0], 2);
    ASSERT_EQ(buff[1], 3);
    ASSERT_EQ(buff.size(), 2);
    ASSERT_EQ(buff.capacity(), 2);
}

TEST(BufferTest, BeginEndFrontBack) {
    CCircularBuffer<char> buff({'a', 'b', 'y', 'z'});
    ASSERT_EQ(*buff.begin(), 'a');
    ASSERT_EQ(*buff.cbegin(), 'a');
    ASSERT_EQ(buff.front(), 'a');
    ASSERT_EQ(buff.back(), 'z');
    ASSERT_EQ(buff.end() - buff.begin(), 4);
    ASSERT_EQ(buff.cend() - buff.cbegin(), 4);
    *buff.begin() = 'x';
    ASSERT_EQ(buff[0], 'x');
}

TEST(BufferTest, AssignmentOperatorBuffer) {
    CCircularBuffer<double> buff1({1, 0.66, 12.34, -4});
    CCircularBuffer<double> buff2({0, 2.66, 2.34, -54});
    buff1 = buff2;
    ASSERT_EQ(buff1, buff2);
}

TEST(BufferTest, AssignmentOperatorInitializerList) {
    CCircularBuffer<double> buff1({1, 0.66, 12.34, -4});
    CCircularBuffer<double> buff2({0, 2.66, 2.34, -54});
    buff1 = {0, 2.66, 2.34, -54};
    ASSERT_EQ(buff1, buff2);
}

TEST(BufferTest, Swap) {
    CCircularBuffer<int> buff1({1, 2, 3});
    CCircularBuffer<int> buff2({4, 5, 6, 7, 8});
    buff1.swap(buff2);
    ASSERT_EQ(buff1[0], 4);
    ASSERT_EQ(buff1[1], 5);
    ASSERT_EQ(buff1[2], 6);
    ASSERT_EQ(buff1[3], 7);
    ASSERT_EQ(buff1[4], 8);
    ASSERT_EQ(buff2[0], 1);
    ASSERT_EQ(buff2[1], 2);
    ASSERT_EQ(buff2[2], 3);
}

TEST(BufferTest, InsertOneElementByOrder) {
    CCircularBuffer<int> buff(5);
    buff.insert(buff.begin(), 112);
    buff.insert(buff.begin() + 1, 0);
    buff.insert(buff.begin() + 2, 3145);
    ASSERT_EQ(buff[0], 112);
    ASSERT_EQ(buff[1], 0);
    ASSERT_EQ(buff[2], 3145);
}

TEST(BufferTest, InsertOneElementInTheBeginning) {
    CCircularBuffer<int> buff(5);
    buff.insert(buff.begin(), 112);
    buff.insert(buff.begin(), 0);
    buff.insert(buff.begin(), 3145);
    ASSERT_EQ(buff[0], 3145);
    ASSERT_EQ(buff[1], 0);
    ASSERT_EQ(buff[2], 112);
}

TEST(BufferTest, InsertNElementsInEmptyBuffer) {
    CCircularBuffer<std::string> buff(5);
    buff.insert(buff.begin(), 3, "112");
    ASSERT_EQ(buff[0], "112");
    ASSERT_EQ(buff[1], "112");
    ASSERT_EQ(buff[2], "112");
    std::cout << buff[4];
}

TEST(BufferTest, InsertNElementsInFullBuffer) {
    CCircularBuffer<int> buff({50, 21, 82, 79, 52, 94});
    buff.insert(buff.begin() + 2, 3, 112);
    ASSERT_EQ(buff[0], 79);
    ASSERT_EQ(buff[1], 52);
    ASSERT_EQ(buff[2], 112);
    ASSERT_EQ(buff.at(3), 112);
    ASSERT_EQ(buff.at(4), 112);
    ASSERT_EQ(buff.at(5), 82);
}

TEST(BufferTest, InsertElementsByIterator) {
    std::vector<int> vec({35, 99, 36, 51, 30});
    CCircularBuffer<int> buff({50, 21, 82, 79, 52, 94});
    buff.insert(buff.begin() + 4, vec.begin() + 1, vec.end());
    ASSERT_EQ(buff[0], 51);
    ASSERT_EQ(buff[1], 30);
    ASSERT_EQ(buff[2], 52);
    ASSERT_EQ(buff[3], 94);
    ASSERT_EQ(buff[4], 99);
    ASSERT_EQ(buff[5], 36);
}

TEST(BufferTest, InsertElementsByInitializerList) {
    CCircularBuffer<int> buff({50, 21, 82, 79, 52, 94});
    buff.insert(buff.begin() + 4, {99, 36, 51, 30});
    ASSERT_EQ(buff[0], 51);
    ASSERT_EQ(buff[1], 30);
    ASSERT_EQ(buff[2], 52);
    ASSERT_EQ(buff[3], 94);
    ASSERT_EQ(buff[4], 99);
    ASSERT_EQ(buff[5], 36);
}

TEST(BufferTest, EraseOneElement) {
    CCircularBuffer<int> buff({64, 30, 39, 92, 11});
    buff.erase(buff.begin());
    ASSERT_EQ(buff[0], 30);
    ASSERT_EQ(buff[1], 39);
    ASSERT_EQ(buff[2], 92);
    ASSERT_EQ(buff[3], 11);
}

TEST(BufferTest, EraseOneElementEmpty) {
    CCircularBuffer<int> buff({12});
    buff.erase(buff.begin());
    ASSERT_TRUE(buff.empty());
    buff.insert(buff.begin(), 21);
    ASSERT_FALSE(buff.empty());
    ASSERT_EQ(buff[0], 21);
}

TEST(BufferTest, EraseMultipleElementsFirstCondition) {
    CCircularBuffer<int> buff(9);
    buff.insert(buff.begin(), {1, 2, 3, 4, 5, 6, 7});
    buff.erase(buff.begin() + 2, buff.begin() + 4);
    ASSERT_EQ(buff[0], 1);
    ASSERT_EQ(buff[1], 2);
    ASSERT_EQ(buff[2], 5);
    ASSERT_EQ(buff[3], 6);
    ASSERT_EQ(buff[4], 7);
    ASSERT_EQ(buff.size(), 5);
}

TEST(BufferTest, EraseMultipleElementsSecondCondition) {
    CCircularBuffer<int> buff(7);
    buff.insert(buff.begin(), {1, 2, 3, 4, 5});
    buff.erase(buff.begin() + 2, buff.begin() + 4);
    ASSERT_EQ(buff[0], 1);
    ASSERT_EQ(buff[1], 2);
    ASSERT_EQ(buff[2], 5);
}

TEST(BufferTest, AssignTwoPointers) {
    CCircularBuffer<double> buff1({1, 0.66, 12.34, -4});
    CCircularBuffer<double> buff2({0, 2.66, 2.34, -54});
    buff1.assign(buff2.begin().get_pointer(), buff2.end().get_pointer());
    ASSERT_EQ(buff1, buff2);
}

TEST(BufferTest, AssignInitializerList) {
    CCircularBuffer<int> buff(5, 1);
    buff.assign({1, 3});
    ASSERT_EQ(buff[0], 1);
    ASSERT_EQ(buff[1], 3);
    ASSERT_EQ(buff.size(), 2);
}

TEST(BufferTest, AssignNElements) {
    CCircularBuffer<int> buff;
    buff.assign(3, 4);
    ASSERT_EQ(buff[0], buff[1]);
    ASSERT_EQ(buff[1], buff[2]);
    ASSERT_EQ(buff[2], 4);
    ASSERT_EQ(buff.size(), 3);
}

TEST(BufferTest, Resize) {
    CCircularBuffer<int> buff;
    ASSERT_EQ(buff.size(), 0);
    ASSERT_EQ(buff.capacity(), 1);
    buff.resize(5);
    buff.insert(buff.begin(), {3, 12121});
    ASSERT_EQ(buff[0], 3);
    ASSERT_EQ(buff[1], 12121);
    ASSERT_EQ(buff[2], 0);
    ASSERT_EQ(buff[3], 0);
    ASSERT_EQ(buff[4], 0);
    ASSERT_EQ(buff.size(), 5);
    ASSERT_EQ(buff.capacity(), 5);
    buff.resize(1);
    ASSERT_EQ(buff[0], 3);
    ASSERT_EQ(buff.size(), 1);
    ASSERT_EQ(buff.capacity(), 5);
}

TEST(BufferTest, Reserve) {
    CCircularBuffer<int> buff;
    ASSERT_EQ(buff.size(), 0);
    ASSERT_EQ(buff.capacity(), 1);
    buff.reserve(5);
    ASSERT_EQ(buff.size(), 0);
    ASSERT_EQ(buff.capacity(), 5);
    buff.reserve(3);
    ASSERT_EQ(buff.size(), 0);
    ASSERT_EQ(buff.capacity(), 5);
    buff.reserve(10);
    ASSERT_EQ(buff.size(), 0);
    ASSERT_EQ(buff.capacity(), 10);
}

TEST(ExtBufferTest, Insert) {
    CExtCircularBuffer<int> buff;
    buff.insert(buff.begin(), 432);
    ASSERT_EQ(buff[0], 432);
    ASSERT_EQ(buff.size(), 1);
    ASSERT_EQ(buff.capacity(), 1);
    buff.insert(buff.begin() + 1, {3, 7, 12, 41});
    ASSERT_EQ(buff[0], 432);
    ASSERT_EQ(buff[1], 3);
    ASSERT_EQ(buff[2], 7);
    ASSERT_EQ(buff[3], 12);
    ASSERT_EQ(buff[4], 41);
    ASSERT_EQ(buff.size(), 5);
    ASSERT_EQ(buff.capacity(), 8);
    buff.reserve(10);
    ASSERT_EQ(buff.size(), 5);
    ASSERT_EQ(buff.capacity(), 10);
}