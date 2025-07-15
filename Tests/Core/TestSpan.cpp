/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#define KTA_ASSUME_TESTING_ENV_
#include <catch2/catch_test_macros.hpp>
#include <Kalantha/Core/Span.hpp>

using namespace kta;

TEST_CASE("Span construction", "[Core.Span]") {
  SECTION("Default construction") {
    Span<int> span;
    REQUIRE(span.data() == nullptr);
    REQUIRE(span.size() == 0);
    REQUIRE(span.empty());
    REQUIRE(span.begin() == span.end());
  }

  SECTION("Construction from C-array") {
    int arr[] = {1, 2, 3, 4, 5};
    Span<int> span(arr);
    REQUIRE(span.data() == arr);
    REQUIRE(span.size() == 5);
    REQUIRE(!span.empty());
    REQUIRE(span[0] == 1);
    REQUIRE(span[4] == 5);
  }

  SECTION("Construction from pointer and length") {
    int arr[] = {10, 20, 30};
    Span<int> span(arr, 3);
    REQUIRE(span.data() == arr);
    REQUIRE(span.size() == 3);
    REQUIRE(!span.empty());
    REQUIRE(span[0] == 10);
    REQUIRE(span[2] == 30);
  }

  SECTION("Construction from pointer and zero length") {
    int arr[] = {1, 2, 3};
    Span<int> span(arr, 0);
    REQUIRE(span.data() == arr);
    REQUIRE(span.size() == 0);
    REQUIRE(span.empty());
  }

  SECTION("Construction from nullptr") {
    Span<int> span(nullptr, 0);
    REQUIRE(span.data() == nullptr);
    REQUIRE(span.size() == 0);
    REQUIRE(span.empty());
  }

  SECTION("Copy construction") {
    int arr[] = {1, 2, 3};
    Span<int> original(arr);
    Span<int> copy(original);
    REQUIRE(copy.data() == original.data());
    REQUIRE(copy.size() == original.size());
    REQUIRE(copy[0] == 1);
    REQUIRE(copy[2] == 3);
  }

  SECTION("Move construction") {
    int arr[] = {1, 2, 3};
    Span<int> original(arr);
    Span<int> moved(std::move(original));
    REQUIRE(moved.data() == arr);
    REQUIRE(moved.size() == 3);
    REQUIRE(moved[0] == 1);
  }
}

TEST_CASE("Span assignment", "[Core.Span]") {
  SECTION("Copy assignment") {
    int arr1[] = {1, 2, 3};
    int arr2[] = {4, 5, 6, 7};
    Span<int> span1(arr1);
    Span<int> span2(arr2);
    
    span1 = span2;
    REQUIRE(span1.data() == arr2);
    REQUIRE(span1.size() == 4);
    REQUIRE(span1[0] == 4);
    REQUIRE(span1[3] == 7);
  }

  SECTION("Move assignment") {
    int arr1[] = {1, 2, 3};
    int arr2[] = {4, 5, 6, 7};
    Span<int> span1(arr1);
    Span<int> span2(arr2);
    
    span1 = std::move(span2);
    REQUIRE(span1.data() == arr2);
    REQUIRE(span1.size() == 4);
    REQUIRE(span1[0] == 4);
    REQUIRE(span1[3] == 7);
  }
}

TEST_CASE("Span element access", "[Core.Span]") {
  int arr[] = {10, 20, 30, 40, 50};
  Span<int> span(arr);

  SECTION("Operator[] access") {
    REQUIRE(span[0] == 10);
    REQUIRE(span[2] == 30);
    REQUIRE(span[4] == 50);
    
    span[1] = 99;
    REQUIRE(span[1] == 99);
    REQUIRE(arr[1] == 99); // Verify original array is modified
  }

  SECTION("at() method") {
    REQUIRE(span.at(0) == 10);
    REQUIRE(span.at(2) == 30);
    REQUIRE(span.at(4) == 50);
    
    span.at(3) = 88;
    REQUIRE(span.at(3) == 88);
    REQUIRE(arr[3] == 88);
  }

  SECTION("front() method") {
    REQUIRE(span.front() == 10);
    
    span.front() = 77;
    REQUIRE(span.front() == 77);
    REQUIRE(arr[0] == 77);
  }

  SECTION("back() method") {
    REQUIRE(span.back() == 50);
    
    span.back() = 66;
    REQUIRE(span.back() == 66);
    REQUIRE(arr[4] == 66);
  }
}

TEST_CASE("Span const element access", "[Core.Span]") {
  int arr[] = {10, 20, 30, 40, 50};
  const Span<int> span(arr);

  SECTION("Const operator[] access") {
    REQUIRE(span[0] == 10);
    REQUIRE(span[2] == 30);
    REQUIRE(span[4] == 50);
  }

  SECTION("Const at() method") {
    REQUIRE(span.at(0) == 10);
    REQUIRE(span.at(2) == 30);
    REQUIRE(span.at(4) == 50);
  }

  SECTION("Const front() method") {
    REQUIRE(span.front() == 10);
  }

  SECTION("Const back() method") {
    REQUIRE(span.back() == 50);
  }
}

TEST_CASE("Span data access", "[Core.Span]") {
  int arr[] = {1, 2, 3, 4};
  Span<int> span(arr);

  SECTION("data() method") {
    int* ptr = span.data();
    REQUIRE(ptr == arr);
    REQUIRE(ptr[0] == 1);
    REQUIRE(ptr[3] == 4);
    
    ptr[1] = 99;
    REQUIRE(span[1] == 99);
  }

  SECTION("const data() method") {
    const Span<int> const_span(span);
    const int* ptr = const_span.data();
    REQUIRE(ptr == arr);
    REQUIRE(ptr[0] == 1);
    REQUIRE(ptr[3] == 4);
  }
}

TEST_CASE("Span properties", "[Core.Span]") {
  SECTION("size() method") {
    int arr[] = {1, 2, 3, 4, 5};
    Span<int> span(arr);
    REQUIRE(span.size() == 5);
    
    Span<int> empty_span;
    REQUIRE(empty_span.size() == 0);
    
    Span<int> single_span(arr, 1);
    REQUIRE(single_span.size() == 1);
  }

  SECTION("size_bytes() method") {
    int arr[] = {1, 2, 3, 4};
    Span<int> span(arr);
    REQUIRE(span.size_bytes() == 4 * sizeof(int));
    
    char char_arr[] = {'a', 'b', 'c'};
    Span<char> char_span(char_arr);
    REQUIRE(char_span.size_bytes() == 3 * sizeof(char));
    
    Span<int> empty_span;
    REQUIRE(empty_span.size_bytes() == 0);
  }

  SECTION("empty() method") {
    Span<int> empty_span;
    REQUIRE(empty_span.empty());
    
    int arr[] = {1};
    Span<int> non_empty(arr);
    REQUIRE(!non_empty.empty());
    
    Span<int> zero_length(arr, 0);
    REQUIRE(zero_length.empty());
  }
}

TEST_CASE("Span iterators", "[Core.Span]") {
  int arr[] = {1, 2, 3, 4, 5};
  Span<int> span(arr);

  SECTION("begin() and end()") {
    auto it = span.begin();
    REQUIRE(*it == 1);
    ++it;
    REQUIRE(*it == 2);
    
    auto end_it = span.end();
    REQUIRE(end_it - span.begin() == 5);
  }

  SECTION("const iterators") {
    const Span<int> const_span(span);
    auto it = const_span.begin();
    REQUIRE(*it == 1);
    
    REQUIRE(const_span.end() - const_span.begin() == 5);
  }

  SECTION("Range-based for loop") {
    int expected[] = {1, 2, 3, 4, 5};
    int i = 0;
    for (const auto& val : span) {
      REQUIRE(val == expected[i++]);
    }
    REQUIRE(i == 5);
  }

  SECTION("Iterator modification") {
    auto it = span.begin();
    *it = 99;
    REQUIRE(span[0] == 99);
    REQUIRE(arr[0] == 99);
  }

  SECTION("Empty span iterators") {
    Span<int> empty_span;
    REQUIRE(empty_span.begin() == empty_span.end());
  }
}

TEST_CASE("Span subspan operations", "[Core.Span]") {
  int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
  Span<int> span(arr);

  SECTION("subspan with offset and count") {
    auto sub = span.subspan(2, 3);
    REQUIRE(sub.size() == 3);
    REQUIRE(sub[0] == 3);
    REQUIRE(sub[1] == 4);
    REQUIRE(sub[2] == 5);
    REQUIRE(sub.data() == arr + 2);
  }

  SECTION("subspan with offset only") {
    auto sub = span.subspan(3);
    REQUIRE(sub.size() == 5);
    REQUIRE(sub[0] == 4);
    REQUIRE(sub[4] == 8);
    REQUIRE(sub.data() == arr + 3);
  }

  SECTION("subspan at beginning") {
    auto sub = span.subspan(0, 3);
    REQUIRE(sub.size() == 3);
    REQUIRE(sub[0] == 1);
    REQUIRE(sub[2] == 3);
    REQUIRE(sub.data() == arr);
  }

  SECTION("subspan at end") {
    auto sub = span.subspan(5, 3);
    REQUIRE(sub.size() == 3);
    REQUIRE(sub[0] == 6);
    REQUIRE(sub[2] == 8);
  }

  SECTION("subspan entire span") {
    auto sub = span.subspan(0, span.size());
    REQUIRE(sub.size() == span.size());
    REQUIRE(sub.data() == span.data());
  }

  SECTION("subspan empty") {
    auto sub = span.subspan(3, 0);
    REQUIRE(sub.size() == 0);
    REQUIRE(sub.empty());
    REQUIRE(sub.data() == arr + 3);
  }

  SECTION("subspan from offset to end") {
    auto sub = span.subspan(7);
    REQUIRE(sub.size() == 1);
    REQUIRE(sub[0] == 8);
  }

  SECTION("subspan from end") {
    auto sub = span.subspan(8);
    REQUIRE(sub.size() == 0);
    REQUIRE(sub.empty());
  }
}

TEST_CASE("Span first and last operations", "[Core.Span]") {
  int arr[] = {1, 2, 3, 4, 5, 6};
  Span<int> span(arr);

  SECTION("first() method") {
    auto first3 = span.first(3);
    REQUIRE(first3.size() == 3);
    REQUIRE(first3[0] == 1);
    REQUIRE(first3[1] == 2);
    REQUIRE(first3[2] == 3);
    REQUIRE(first3.data() == arr);
  }

  SECTION("last() method") {
    auto last3 = span.last(3);
    REQUIRE(last3.size() == 3);
    REQUIRE(last3[0] == 4);
    REQUIRE(last3[1] == 5);
    REQUIRE(last3[2] == 6);
    REQUIRE(last3.data() == arr + 3);
  }

  SECTION("first(0)") {
    auto first0 = span.first(0);
    REQUIRE(first0.size() == 0);
    REQUIRE(first0.empty());
    REQUIRE(first0.data() == arr);
  }

  SECTION("last(0)") {
    auto last0 = span.last(0);
    REQUIRE(last0.size() == 0);
    REQUIRE(last0.empty());
    REQUIRE(last0.data() == arr + 6);
  }

  SECTION("first entire span") {
    auto first_all = span.first(span.size());
    REQUIRE(first_all.size() == span.size());
    REQUIRE(first_all.data() == span.data());
  }

  SECTION("last entire span") {
    auto last_all = span.last(span.size());
    REQUIRE(last_all.size() == span.size());
    REQUIRE(last_all.data() == span.data());
  }
}

TEST_CASE("Span drop operations", "[Core.Span]") {
  int arr[] = {1, 2, 3, 4, 5, 6, 7};
  Span<int> span(arr);

  SECTION("drop_front() method") {
    auto dropped = span.drop_front(2);
    REQUIRE(dropped.size() == 5);
    REQUIRE(dropped[0] == 3);
    REQUIRE(dropped[4] == 7);
    REQUIRE(dropped.data() == arr + 2);
  }

  SECTION("drop_back() method") {
    auto dropped = span.drop_back(2);
    REQUIRE(dropped.size() == 5);
    REQUIRE(dropped[0] == 1);
    REQUIRE(dropped[4] == 5);
    REQUIRE(dropped.data() == arr);
  }

  SECTION("drop_front(0)") {
    auto dropped = span.drop_front(0);
    REQUIRE(dropped.size() == span.size());
    REQUIRE(dropped.data() == span.data());
  }

  SECTION("drop_back(0)") {
    auto dropped = span.drop_back(0);
    REQUIRE(dropped.size() == span.size());
    REQUIRE(dropped.data() == span.data());
  }

  SECTION("drop_front entire span") {
    auto dropped = span.drop_front(span.size());
    REQUIRE(dropped.size() == 0);
    REQUIRE(dropped.empty());
    REQUIRE(dropped.data() == arr + 7);
  }

  SECTION("drop_back entire span") {
    auto dropped = span.drop_back(span.size());
    REQUIRE(dropped.size() == 0);
    REQUIRE(dropped.empty());
    REQUIRE(dropped.data() == arr);
  }

  SECTION("Combined drop operations") {
    auto dropped = span.drop_front(2).drop_back(2);
    REQUIRE(dropped.size() == 3);
    REQUIRE(dropped[0] == 3);
    REQUIRE(dropped[1] == 4);
    REQUIRE(dropped[2] == 5);
  }
}

TEST_CASE("Span with different types", "[Core.Span]") {
  SECTION("char span") {
    char arr[] = {'a', 'b', 'c', 'd'};
    Span<char> span(arr);
    REQUIRE(span.size() == 4);
    REQUIRE(span[0] == 'a');
    REQUIRE(span[3] == 'd');
    REQUIRE(span.size_bytes() == 4);
  }

  SECTION("double span") {
    double arr[] = {1.1, 2.2, 3.3};
    Span<double> span(arr);
    REQUIRE(span.size() == 3);
    REQUIRE(span[0] == 1.1);
    REQUIRE(span[2] == 3.3);
    REQUIRE(span.size_bytes() == 3 * sizeof(double));
  }

  SECTION("Custom struct span") {
    struct TestStruct {
      int value;
      TestStruct(int v) : value(v) {}
      bool operator==(const TestStruct& other) const {
        return value == other.value;
      }
    };

    TestStruct arr[] = {TestStruct(10), TestStruct(20), TestStruct(30)};
    Span<TestStruct> span(arr);
    REQUIRE(span.size() == 3);
    REQUIRE(span[0].value == 10);
    REQUIRE(span[2].value == 30);
    REQUIRE(span.size_bytes() == 3 * sizeof(TestStruct));
  }
}

TEST_CASE("ReadOnlySpan alias", "[Core.Span]") {
  int arr[] = {1, 2, 3, 4, 5};
  
  SECTION("ReadOnlySpan basic functionality") {
    ReadOnlySpan<int> span(arr);
    REQUIRE(span.size() == 5);
    REQUIRE(span[0] == 1);
    REQUIRE(span[4] == 5);
    REQUIRE(!span.empty());
  }

  SECTION("ReadOnlySpan subspan operations") {
    ReadOnlySpan<int> span(arr);
    auto sub = span.subspan(1, 3);
    REQUIRE(sub.size() == 3);
    REQUIRE(sub[0] == 2);
    REQUIRE(sub[2] == 4);
  }
}

TEST_CASE("Span edge cases", "[Core.Span]") {
  SECTION("Single element span") {
    int arr[] = {42};
    Span<int> span(arr);
    REQUIRE(span.size() == 1);
    REQUIRE(span[0] == 42);
    REQUIRE(span.front() == 42);
    REQUIRE(span.back() == 42);
    REQUIRE(span.first(1).size() == 1);
    REQUIRE(span.last(1).size() == 1);
  }

  SECTION("Chained operations") {
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Span<int> span(arr);
    
    auto result = span.drop_front(2).drop_back(2).subspan(1, 4);
    REQUIRE(result.size() == 4);
    REQUIRE(result[0] == 4);
    REQUIRE(result[3] == 7);
  }
}

TEST_CASE("Span modification through iterators", "[Core.Span]") {
  int arr[] = {1, 2, 3, 4, 5};
  Span<int> span(arr);

  SECTION("Modify through iterator") {
    for (auto& val : span) {
      val *= 2;
    }
    
    REQUIRE(span[0] == 2);
    REQUIRE(span[1] == 4);
    REQUIRE(span[2] == 6);
    REQUIRE(span[3] == 8);
    REQUIRE(span[4] == 10);
    
    // Verify original array is modified
    REQUIRE(arr[0] == 2);
    REQUIRE(arr[4] == 10);
  }
}

