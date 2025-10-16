#include "vector.hpp."
#include "build/googletest-src/googletest/include/gtest/gtest.h"

TEST(vector, empty) {
    MyContainer::Vector<int> vector;

    EXPECT_TRUE(vector.empty());
}

TEST(vector, getCount) {
    MyContainer::Vector<int> vector;

    vector.push_back(1);
    vector.push_back(2);

    EXPECT_FALSE(vector.empty());
    EXPECT_EQ(vector.getCount(), 2);
}

TEST(vector, push_back) {
    MyContainer::Vector<int> vector;

    vector.push_back(5);

    EXPECT_EQ(vector.getCount(), 1);
}

TEST(vector, insert) {
    MyContainer::Vector<int> vector;

    vector.push_back(1);
    vector.push_back(2);
    vector.push_back(4);
    vector.insert(2, 3);

    EXPECT_EQ(vector.getCount(), 4);
    EXPECT_EQ(vector[2], 3);
}

TEST(vector, subscript) {
    MyContainer::Vector<int> vector;

    vector.push_back(2);

    EXPECT_EQ(vector[0], 2);
}

TEST(vector, erase) {
    MyContainer::Vector<int> vector;

    vector.push_back(1);
    vector.push_back(2);
    vector.push_back(3);
    vector.push_back(4);

    vector.erase(1, 2);

    EXPECT_EQ(vector[1], 4);
}

TEST(vector, copy_constructor) {
    MyContainer::Vector<int> origin;

    origin.push_back(2);

    MyContainer::Vector<int> copy;

    copy = origin;

    EXPECT_EQ(copy[0], origin[0]);
}

TEST(vector, copy_assignment_operator) {
    MyContainer::Vector<int> origin;

    origin.push_back(3);

    MyContainer::Vector<int> copy = origin;

    copy = std::move(origin);

    EXPECT_TRUE(origin.empty());
}

TEST(vector, move_assignment_operator) {
    MyContainer::Vector<int> origin;

    origin.push_back(4);

    MyContainer::Vector<int> copy;

    copy = std::move(origin);

    EXPECT_TRUE(origin.empty());
}

TEST(vector, output) {
    MyContainer::Vector<int> vector;

    vector.push_back(1);
    vector.push_back(2);
    vector.push_back(3);
    vector.push_back(4);

    std::stringstream buffer;
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::cout.rdbuf(buffer.rdbuf());
    std::cout << vector;
    std::cout.rdbuf(oldCoutStreamBuf);

    EXPECT_EQ(buffer.str(), "1 2 3 4 ");
}

TEST(vector, iterator) {
    MyContainer::Vector<int> vector;

    vector.push_back(1);
    vector.push_back(2);
    vector.push_back(3);
    vector.push_back(4);

    std::stringstream buffer;
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::cout.rdbuf(buffer.rdbuf());

    for (const int item : vector) {
        std::cout << item << " ";
    }

    std::cout.rdbuf(oldCoutStreamBuf);

    EXPECT_EQ(buffer.str(), "1 2 3 4 ");
}