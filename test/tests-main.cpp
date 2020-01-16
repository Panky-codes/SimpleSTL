#define CATCH_CONFIG_MAIN
#include <algorithm>

#include "Vector.h"
#include "catch.hpp"
template <typename T>
void printvector(PR::vector<T> to_print) {
    for (const auto& elem : to_print) {
        std::cout << elem << '\n';
    }
}
#define RUN_ALL_TESTS

#if defined(RUN_ALL_TESTS)
TEST_CASE("Simple initializer list constructor ") {
    PR::vector<int> vec{1, 2};

    REQUIRE(vec[1] == 2);
}

TEST_CASE("Copy constructor") {
    PR::vector<int> vec1{1, 2};
    PR::vector<int> vec{vec1};

    REQUIRE(vec == vec1);
}

TEST_CASE("Assignment == operator check") {
    PR::vector<int> vec1{1, 2};
    PR::vector<int> vec{vec1};

    vec[1] = 3;
    REQUIRE(!(vec == vec1));
}

TEST_CASE("Copy assignment") {
    PR::vector<int> vec;
    PR::vector<int> vec1{1, 2};
    vec = vec1;

    REQUIRE(vec[1] == vec1[1]);
    REQUIRE(vec[0] == vec1[0]);
}

TEST_CASE("front() and back() (indirectly tests at()) ") {
    PR::vector<int> vec1{1, 2};
    vec1.back() = 5;
    vec1.front() = 2;

    REQUIRE(vec1[0] == 2);
    REQUIRE(vec1[1] == 5);
}

TEST_CASE("at() exception out of range ") {
    PR::vector<int> vec1{1, 2};

    REQUIRE_THROWS_AS(vec1.at(2), std::out_of_range);
}

TEST_CASE("begin and end iterator check with find algorithm") {
    PR::vector<int> vec1{1, 2, 5, 3};
    auto expected_value = std::find(vec1.begin(), vec1.end(), vec1.back());

    REQUIRE(*expected_value == vec1.back());
}

TEST_CASE("Push back functionality") {
    PR::vector<int> vec1{1, 2, 5, 3};
    int actual_value = 10;
    // Do two push backs to verify it doesn't reallocate both the
    // times
    vec1.push_back(actual_value);
    vec1.push_back(actual_value);

    REQUIRE(vec1.back() == actual_value);
    REQUIRE(vec1.front() == 1);
    REQUIRE(vec1.size() == 6);
    REQUIRE(vec1.capacity() == 8);
}

TEST_CASE("Push back functionality with string container") {
    PR::vector<std::string> vec1{"Hello", "World"};
    std::string actual_value = "!";
    // Do two push backs to verify it doesn't reallocate both the
    // times
    vec1.push_back(actual_value);
    vec1.push_back(actual_value);

    REQUIRE(vec1.back() == actual_value);
    REQUIRE(vec1.size() == 4);
    REQUIRE(vec1.capacity() == 4);
}

TEST_CASE("emplace back functionality") {
    PR::vector<std::string> vec1(2, std::string("Yello"));
    // Do two push backs to verify it doesn't reallocate both the
    // times
    vec1.emplace_back("Yello");

    REQUIRE(vec1.back() == "Yello");
    REQUIRE(vec1.size() == 3);
    REQUIRE(vec1.capacity() == 4);
}
TEST_CASE("Shrink to fit") {
    PR::vector<int> vec1{1, 2, 5, 3};
    int actual_value = 10;
    vec1.push_back(actual_value);
    vec1.shrink_to_fit();

    REQUIRE(vec1.capacity() == 5);
}

TEST_CASE("Emplace check") {
    PR::vector<int> vec1{1, 2, 5, 3};
    auto actual_iter = vec1.emplace((vec1.begin() + 1), 4);
    REQUIRE(*actual_iter == 4);
    //    REQUIRE(vec1.at(2) == 2);
    REQUIRE(vec1.at(3) == 5);
    REQUIRE(vec1.at(4) == 3);
}

TEST_CASE("Reserve capacity check") {
    PR::vector<int> vec1{1, 2, 5, 3};
    REQUIRE(vec1.capacity() == 4);
    // Should not reallocate for this reserve
    vec1.reserve(3);
    REQUIRE(vec1.capacity() == 4);

    vec1.reserve(6);
    REQUIRE(vec1.capacity() == 6);
}
TEST_CASE("Emplace string move functionality") {
    PR::vector<std::string> vec1{"World"};
    std::string vec2{"Hello"};
    vec1.emplace(vec1.begin(), std::move(vec2));
    REQUIRE(vec1[0] == "Hello");
    REQUIRE(vec1[1] == "World");
}

TEST_CASE("Emplace vector of vector functionality") {
    PR::vector<PR::vector<int>> vec1;
    PR::vector<int> vec2{1, 2, 3, 4};
    vec1.emplace(vec1.begin(), std::move(vec2));

    REQUIRE(vec1[0][0] == 1);
    REQUIRE(vec1[0][1] == 2);
    REQUIRE(vec1[0][3] == 4);
}
TEST_CASE("Vector empty functionality") {
    PR::vector<PR::vector<int>> vec1;

    REQUIRE(vec1.empty());
}
TEST_CASE("Vector clear functionality") {
    PR::vector<int> vec1{1, 2, 3};
    vec1.clear();

    REQUIRE(vec1.empty());
}
TEST_CASE("Vector erase single element") {
    PR::vector<int> vec1{1, 2, 3};
    auto iter_pos = vec1.erase(vec1.begin() + 1);

    REQUIRE(*iter_pos == 3);
    REQUIRE(vec1.size() == 2);
}
TEST_CASE("Vector erase range of elements") {
    PR::vector<int> vec1{1, 2, 3, 4};
    auto iter_pos = vec1.erase(vec1.begin() + 1, vec1.end() - 1);

    REQUIRE(*iter_pos == 4);
    REQUIRE(vec1.size() == 2);
}
TEST_CASE("Vector erase range of elements until the end") {
    PR::vector<int> vec1{1, 2, 3, 4};
    auto iter_pos = vec1.erase(vec1.begin() + 1, vec1.end());

    REQUIRE(iter_pos == vec1.end());
    REQUIRE(vec1.size() == 1);
}
TEST_CASE("Vector insert single element") {
    PR::vector<int> vec1{1, 2, 3};
    int value = 8;
    auto iter = vec1.insert(vec1.cbegin() + 1, value);

    REQUIRE(vec1.at(0) == 1);
    REQUIRE(*iter == value);
    REQUIRE(vec1.at(2) == 2);
    REQUIRE(vec1.at(3) == 3);
    REQUIRE(vec1.size() == 4);
}

TEST_CASE("Vector insert single element in the middle with move") {
    PR::vector<std::string> vec1{"Hello", "World", "!!"};

    auto iter = vec1.insert(vec1.cbegin() + 1, "C++");

    REQUIRE(vec1.at(0) == "Hello");
    REQUIRE(vec1.at(1) == "C++");
    REQUIRE(vec1.at(2) == "World");
    REQUIRE(vec1.at(3) == "!!");
    REQUIRE(*iter == "C++");
    REQUIRE(vec1.size() == 4);
}

TEST_CASE("Vector insert N counts of string elements") {
    PR::vector<std::string> vec1{"Hello", "World", "!!"};

    auto iter = vec1.insert(vec1.cbegin() + 2, 1, "C++");

    REQUIRE(*iter == "C++");
    REQUIRE(vec1.at(0) == "Hello");
    REQUIRE(vec1.at(1) == "World");
    REQUIRE(vec1.at(2) == "C++");
    REQUIRE(vec1.at(3) == "!!");
    REQUIRE(vec1.size() == 4);
}

TEST_CASE("Insert N counts of vector elements at the end") {
    PR::vector<PR::vector<int>> vec1{{1, 2}, {3, 4}, {5, 6}};

    auto iter = vec1.insert(vec1.cbegin() + 3, 1, {7, 8});

    REQUIRE(vec1.at(0) == PR::vector<int>{1, 2});
    REQUIRE(vec1.at(1) == PR::vector<int>{3, 4});
    REQUIRE(vec1.at(2) == PR::vector<int>{5, 6});
    REQUIRE(*iter == PR::vector<int>{7, 8});
    REQUIRE(vec1.size() == 4);
}
TEST_CASE("Insert N counts of vector elements in the middle") {
    PR::vector<PR::vector<int>> vec1{{1, 2}, {3, 4}, {5, 6}};

    auto iter = vec1.insert(vec1.cbegin() + 1, 2, {7, 8});
    REQUIRE(vec1.at(0) == PR::vector<int>{1, 2});
    REQUIRE(vec1.at(1) == PR::vector<int>{7, 8});
    REQUIRE(vec1.at(2) == PR::vector<int>{7, 8});
    REQUIRE(vec1.at(3) == PR::vector<int>{3, 4});
    REQUIRE(vec1.at(4) == PR::vector<int>{5, 6});
    REQUIRE(*iter == PR::vector<int>{7, 8});
    REQUIRE(vec1.size() == 5);
}
TEST_CASE("Insert N count of integer elements ") {
    PR::vector<int> vec1{1, 2, 3, 4};

    auto iter = vec1.insert(vec1.cbegin() + 4, 2, 5);

    REQUIRE(*iter == 5);
    REQUIRE(vec1.size() == 6);
}

TEST_CASE("Insert a vector of ints inside an another vector") {
    PR::vector<int> vec1{7, 8, 9};
    PR::vector<int> vec2{1, 2, 3};

    auto iter = vec1.insert(vec1.cbegin() + 0, vec2.begin(), vec2.end());
    REQUIRE(*iter == 1);
    REQUIRE(*++iter == 2);
    REQUIRE(*++iter == 3);
    REQUIRE(vec1.at(3) == 7);
    REQUIRE(vec1.at(4) == 8);
    REQUIRE(vec1.at(5) == 9);
}

TEST_CASE("Insert a vector of strings inside an another vector") {
    PR::vector<std::string> vec1{"Hello", "World", "!!"};
    PR::vector<std::string> vec2{"Yello", "Worlds", "!!!"};

    auto iter = vec1.insert(vec1.cbegin() + 3, vec2.begin(), vec2.end());
    REQUIRE(*iter == "Yello");
    REQUIRE(*++iter == "Worlds");
    REQUIRE(*++iter == "!!!");
}

TEST_CASE("Insert a vector of strings inside an another vector at the end") {
    PR::vector<std::string> vec1{"Hello", "World", "!!"};
    PR::vector<std::string> vec2{"Yello", "Worlds", "!!!"};

    auto iter = vec1.insert(vec1.cbegin() + 3, vec2.begin(), vec2.end());
    REQUIRE(*iter == "Yello");
    REQUIRE(*++iter == "Worlds");
    REQUIRE(*++iter == "!!!");
}

TEST_CASE("Swap vector of ints") {
    PR::vector<int> vec1{1, 2, 3};
    PR::vector<int> vec2{3, 2, 1, 64};

    vec1.swap(vec2);
    REQUIRE(vec1[0] == 3);
    REQUIRE(vec1[1] == 2);
    REQUIRE(vec1[2] == 1);
    REQUIRE(vec1[3] == 64);
    REQUIRE(vec2[0] == 1);
    REQUIRE(vec2[1] == 2);
    REQUIRE(vec2[2] == 3);
}

TEST_CASE("Swap vector of strings") {
    PR::vector<std::string> vec1{"Hello", "World", "!!"};
    PR::vector<std::string> vec2{"Yello", "Worlds", "!!!"};

    vec1.swap(vec2);
    REQUIRE(vec1[0] == "Yello");
    REQUIRE(vec1[1] == "Worlds");
    REQUIRE(vec1[2] == "!!!");
}

TEST_CASE("Resize vector of ints") {
    PR::vector<int> vec1{1, 2, 3};

    vec1.resize(5, 6);
    REQUIRE(vec1.size() == 5);
    REQUIRE(vec1.back() == 6);
    vec1.resize(2);
    REQUIRE(vec1.size() == 2);
}

TEST_CASE("Reverse iterator check") {
    PR::vector<int> vec1{1, 2, 3};
    int reverse_counter = vec1.size();

    for (PR::vector<int>::reverse_iterator i = vec1.rbegin(); i != vec1.rend();
         ++i) {
        REQUIRE(*i == vec1[--reverse_counter]);
    }
}

#endif
