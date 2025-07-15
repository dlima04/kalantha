/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#define KTA_ASSUME_TESTING_ENV_
#include <catch2/catch_test_macros.hpp>
#include <Kalantha/Core/Array.hpp>
#include <string>

/*
* Unit tests here are minimal.
*/

using namespace kta;

TEST_CASE("Array construction", "[Core.Array]") {
  SECTION("Default construction") {
    Array<int, 5> arr;
    REQUIRE(arr.size() == 5);
    REQUIRE(!arr.empty());
    REQUIRE(arr[0] == 0); // Default constructed
  }

  SECTION("Zero-sized array") {
    Array<int, 0> arr;
    REQUIRE(arr.size() == 0);
    REQUIRE(arr.empty());
  }

  SECTION("Construction with arguments") {
    Array<int, 3> arr(1, 2, 3);
    REQUIRE(arr.size() == 3);
    REQUIRE(arr[0] == 1);
    REQUIRE(arr[1] == 2);
    REQUIRE(arr[2] == 3);
  }

  SECTION("Construction from C-array") {
    int c_arr[] = {10, 20, 30};
    Array<int, 3> arr(c_arr);
    REQUIRE(arr[0] == 10);
    REQUIRE(arr[1] == 20);
    REQUIRE(arr[2] == 30);
  }

  SECTION("Copy construction") {
    Array<int, 3> original(1, 2, 3);
    Array<int, 3> copy(original);
    REQUIRE(copy[0] == 1);
    REQUIRE(copy[1] == 2);
    REQUIRE(copy[2] == 3);
  }
}

TEST_CASE("Array element access", "[Core.Array]") {
  Array<int, 4> arr(10, 20, 30, 40);

  SECTION("Operator[] access") {
    REQUIRE(arr[0] == 10);
    REQUIRE(arr[3] == 40);

    arr[1] = 99;
    REQUIRE(arr[1] == 99);
  }

  SECTION("at() method") {
    REQUIRE(arr.at(0) == 10);
    REQUIRE(arr.at(3) == 40);

    arr.at(2) = 88;
    REQUIRE(arr.at(2) == 88);
  }

  SECTION("front() and back()") {
    REQUIRE(arr.front() == 10);
    REQUIRE(arr.back() == 40);

    arr.front() = 77;
    arr.back() = 66;
    REQUIRE(arr.front() == 77);
    REQUIRE(arr.back() == 66);
  }
}

TEST_CASE("Array data access", "[Core.Array]") {
  Array<int, 3> arr(1, 2, 3);

  SECTION("data() method") {
    int* ptr = arr.data();
    REQUIRE(ptr[0] == 1);
    REQUIRE(ptr[1] == 2);
    REQUIRE(ptr[2] == 3);

    ptr[1] = 99;
    REQUIRE(arr[1] == 99);
  }

  SECTION("const data() method") {
    const Array<int, 3> const_arr(arr);
    const int* ptr = const_arr.data();
    REQUIRE(ptr[0] == 1);
    REQUIRE(ptr[2] == 3);
  }
}

TEST_CASE("Array iterators", "[Core.Array]") {
  Array<int, 4> arr(1, 2, 3, 4);

  SECTION("begin() and end()") {
    auto it = arr.begin();
    REQUIRE(*it == 1);
    ++it;
    REQUIRE(*it == 2);

    auto end_it = arr.end();
    REQUIRE(end_it - arr.begin() == 4);
  }

  SECTION("const iterators") {
    const Array<int, 4> const_arr(arr);
    auto it = const_arr.begin();
    REQUIRE(*it == 1);

    // Test iterator distance
    REQUIRE(const_arr.end() - const_arr.begin() == 4);
  }

  SECTION("Range-based for loop") {
    int expected[] = {1, 2, 3, 4};
    int i = 0;
    for (const auto& val : arr) {
      REQUIRE(val == expected[i++]);
    }
  }
}

TEST_CASE("Array fill method", "[Core.Array]") {
  Array<int, 5> arr;

  arr.fill(42);

  for (usize i = 0; i < arr.size(); ++i) {
    REQUIRE(arr[i] == 42);
  }
}

TEST_CASE("Array properties", "[Core.Array]") {
  SECTION("size() method") {
    Array<int, 10> arr;
    REQUIRE(arr.size() == 10);

    Array<double, 1> single;
    REQUIRE(single.size() == 1);
  }

  SECTION("empty() method") {
    Array<int, 0> empty_arr;
    REQUIRE(empty_arr.empty());

    Array<int, 1> non_empty;
    REQUIRE(!non_empty.empty());
  }
}

TEST_CASE("Array with custom types", "[Core.Array]") {
  struct TestStruct {
    int value;
    TestStruct() : value(0) {}
    TestStruct(int v) : value(v) {}
    bool operator==(const TestStruct& other) const {
      return value == other.value;
    }
  };

  SECTION("Custom type construction") {
    Array<TestStruct, 3> arr;
    REQUIRE(arr[0].value == 0);

    arr[0] = TestStruct(42);
    REQUIRE(arr[0].value == 42);
  }

  SECTION("Custom type with arguments") {
    Array<TestStruct, 2> arr(TestStruct(10), TestStruct(20));
    REQUIRE(arr[0].value == 10);
    REQUIRE(arr[1].value == 20);
  }
}

