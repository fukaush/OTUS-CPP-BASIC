#include "list.hpp"
#include "build/googletest-src/googletest/include/gtest/gtest.h"

TEST(list, empty) {
    MyContainer::List<int> list;

    EXPECT_TRUE(list.empty());
}

TEST(list, push_back) {
    MyContainer::List<int> list;
    MyContainer::Node<int>* item = list.push_back(5);

    EXPECT_FALSE(list.empty());
    EXPECT_EQ(list.size(), 1);
    EXPECT_EQ(item->data, 5);
}

TEST(list, size) {
    MyContainer::List<int> list;
    MyContainer::Node<int>* a = list.push_back(1);
    MyContainer::Node<int>* b = list.push_back(2);

    EXPECT_EQ(list.size(), 2);
}

TEST(list, prepend) {
    MyContainer::List<int> list;
    MyContainer::Node<int>* a = list.push_back(1);
    MyContainer::Node<int>* b = list.push_back(2);
    MyContainer::Node<int>* d = list.push_back(4);
    MyContainer::Node<int>* e = list.push_back(5);
    MyContainer::Node<int>* c = list.prepend(3, d);

    EXPECT_EQ(d->prev, c);
}

TEST(list, append) {
    MyContainer::List<int> list;
    MyContainer::Node<int>* a = list.push_back(1);
    MyContainer::Node<int>* b = list.push_back(2);
    MyContainer::Node<int>* c = list.push_back(3);
    MyContainer::Node<int>* e = list.push_back(5);
    MyContainer::Node<int>* d = list.append(4, c);

    EXPECT_EQ(d->prev, c);
}

TEST(list, erase) {
    MyContainer::List<int> list;
    MyContainer::Node<int>* a = list.push_back(1);
    MyContainer::Node<int>* b = list.push_back(2);
    MyContainer::Node<int>* c = list.push_back(3);
    MyContainer::Node<int>* d = list.push_back(4);
    MyContainer::Node<int>* e = list.push_back(5);

    list.erase(c);

    EXPECT_EQ(list.size(), 4);
}

TEST(list, front) {
    MyContainer::List<int> list;
    MyContainer::Node<int>* a = list.push_back(1);
    MyContainer::Node<int>* b = list.push_back(2);

    EXPECT_EQ(list.front(), a);
}

TEST(list, back) {
    MyContainer::List<int> list;
    MyContainer::Node<int>* a = list.push_back(1);
    MyContainer::Node<int>* b = list.push_back(2);

    EXPECT_EQ(list.back(), b);
}

TEST(list, output) {
    MyContainer::List<int> list;
    MyContainer::Node<int>* a = list.push_back(1);
    MyContainer::Node<int>* b = list.push_back(2);
    MyContainer::Node<int>* c = list.push_back(3);
    MyContainer::Node<int>* d = list.push_back(4);
    MyContainer::Node<int>* e = list.push_back(5);

    std::stringstream buffer;
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::cout.rdbuf(buffer.rdbuf());
    std::cout << list;
    std::cout.rdbuf(oldCoutStreamBuf);

    EXPECT_EQ(buffer.str(), "1 2 3 4 5 ");
}