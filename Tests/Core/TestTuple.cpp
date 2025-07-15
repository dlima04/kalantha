/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#define KTA_ASSUME_TESTING_ENV_
#include <catch2/catch_test_macros.hpp>
#include <Kalantha/Core/Tuple.hpp>

#include <iostream>
#include <string>
#include <memory>
#include <type_traits>
#include <concepts>

using namespace kta;

struct NonCopyable {
  int value;
  NonCopyable(int v) : value(v) {}
  NonCopyable(const NonCopyable&) = delete;
  NonCopyable& operator=(const NonCopyable&) = delete;
  NonCopyable(NonCopyable&&) = default;
  NonCopyable& operator=(NonCopyable&&) = default;
};

struct Copyable {
  int value;
  Copyable(int v) : value(v) {}
  Copyable(const Copyable&) = default;
  Copyable& operator=(const Copyable&) = default;
  Copyable(Copyable&&) = default;
  Copyable& operator=(Copyable&&) = default;
};

// Test helper for constexpr evaluation
template<typename T>
constexpr bool test_constexpr_construction() {
  [[maybe_unused]] T t{};
  return true;
}

TEST_CASE("NthType Basic type access", "[Core.Tuple]") {
  REQUIRE(std::is_same_v<NthType<0, int, double, std::string>, int>);
  REQUIRE(std::is_same_v<NthType<1, int, double, std::string>, double>);
  REQUIRE(std::is_same_v<NthType<2, int, double, std::string>, std::string>);
}

TEST_CASE("NthType Single type access", "[Core.Tuple]") {
  REQUIRE(std::is_same_v<NthType<0, int>, int>);
}

TEST_CASE("NthType Complex types", "[Core.Tuple]") {
  using T0 = NthType<0, std::unique_ptr<int>, std::string&, const double*>;
  using T1 = NthType<1, std::unique_ptr<int>, std::string&, const double*>;
  using T2 = NthType<2, std::unique_ptr<int>, std::string&, const double*>;

  REQUIRE(std::is_same_v<T0, std::unique_ptr<int>>);
  REQUIRE(std::is_same_v<T1, std::string&>);
  REQUIRE(std::is_same_v<T2, const double*>);
}

TEST_CASE("Empty TupleStorage", "[Core.Tuple]") {
  [[maybe_unused]] TupleStorage<> empty;
  // Should compile and construct without issues
  REQUIRE(true);
}

TEST_CASE("Single element storage", "[Core.Tuple]") {
  TupleStorage<int> storage(42);
  REQUIRE(storage.head_ == 42);
}

TEST_CASE("Multi-element storage construction", "[Core.Tuple]") {
  TupleStorage<int, double, std::string> storage(42, 3.14, "hello");
  REQUIRE(storage.head_ == 42);
  REQUIRE(storage.tail_.head_ == 3.14);
  REQUIRE(storage.tail_.tail_.head_ == "hello");
}

TEST_CASE("TupleStorage Copy construction", "[Core.Tuple]") {
  TupleStorage<int, double> original(42, 3.14);
  TupleStorage<int, double> copy(original);

  REQUIRE(copy.head_ == 42);
  REQUIRE(copy.tail_.head_ == 3.14);
}

TEST_CASE("TupleStorage Move construction", "[Core.Tuple]") {
  TupleStorage<std::string, int> original("hello", 42);
  TupleStorage<std::string, int> moved(std::move(original));

  REQUIRE(moved.head_ == "hello");
  REQUIRE(moved.tail_.head_ == 42);
}

TEST_CASE("TupleStorage Assignment operators", "[Core.Tuple]") {
  TupleStorage<int, double> storage1(1, 1.0);
  TupleStorage<int, double> storage2(2, 2.0);

  storage1 = storage2;
  REQUIRE(storage1.head_ == 2);
  REQUIRE(storage1.tail_.head_ == 2.0);

  TupleStorage<int, double> storage3(3, 3.0);
  storage1 = std::move(storage3);
  REQUIRE(storage1.head_ == 3);
  REQUIRE(storage1.tail_.head_ == 3.0);
}

TEST_CASE("TupleStorage Constexpr construction", "[Core.Tuple]") {
  constexpr TupleStorage<int, double> storage(42, 3.14);
  static_assert(storage.head_ == 42);
  static_assert(storage.tail_.head_ == 3.14);
}

TEST_CASE("Access first element", "[Core.Tuple]") {
  TupleStorage<int, double, std::string> storage(42, 3.14, "hello");

  auto& first = NthItemAccessor<0, int, double, std::string>::access(storage);
  REQUIRE(first == 42);
  REQUIRE(std::is_same_v<decltype(first), int&>);
}

TEST_CASE("Access middle element", "[Core.Tuple]") {
  TupleStorage<int, double, std::string> storage(42, 3.14, "hello");

  auto& second = NthItemAccessor<1, int, double, std::string>::access(storage);
  REQUIRE(second == 3.14);
  REQUIRE(std::is_same_v<decltype(second), double&>);
}

TEST_CASE("Access last element", "[Core.Tuple]") {
  TupleStorage<int, double, std::string> storage(42, 3.14, "hello");

  auto& third = NthItemAccessor<2, int, double, std::string>::access(storage);
  REQUIRE(third == "hello");
  REQUIRE(std::is_same_v<decltype(third), std::string&>);
}

TEST_CASE("Const access", "[Core.Tuple]") {
  const TupleStorage<int, double> storage(42, 3.14);

  const auto& first = NthItemAccessor<0, int, double>::access(storage);
  const auto& second = NthItemAccessor<1, int, double>::access(storage);

  REQUIRE(first == 42);
  REQUIRE(second == 3.14);
  REQUIRE(std::is_same_v<decltype(first), const int&>);
  REQUIRE(std::is_same_v<decltype(second), const double&>);
}

TEST_CASE("Modification through accessor", "[Core.Tuple]") {
  TupleStorage<int, std::string> storage(42, "hello");

  auto& first = NthItemAccessor<0, int, std::string>::access(storage);
  auto& second = NthItemAccessor<1, int, std::string>::access(storage);

  first = 100;
  second = "world";

  REQUIRE(storage.head_ == 100);
  REQUIRE(storage.tail_.head_ == "world");
}

TEST_CASE("Default construction", "[Core.Tuple]") {
  Tuple<int, double, std::string> t;
  // Should compile and construct
  REQUIRE(true);
}

TEST_CASE("Value construction", "[Core.Tuple]") {
  Tuple<int, double, std::string> t(42, 3.14, "hello");
  REQUIRE(t.get<0>() == 42);
  REQUIRE(t.get<1>() == 3.14);
  REQUIRE(t.get<2>() == "hello");
}

TEST_CASE("Perfect forwarding construction", "[Core.Tuple]") {
  std::string s = "hello";
  Tuple<int, std::string> t1(42, s);  // Copy
  Tuple<int, std::string> t2(42, std::move(s));  // Move

  REQUIRE(t1.get<0>() == 42);
  REQUIRE(t1.get<1>() == "hello");
  REQUIRE(t2.get<0>() == 42);
  REQUIRE(t2.get<1>() == "hello");
}

TEST_CASE("Copy construction", "[Core.Tuple]") {
  Tuple<int, double> original(42, 3.14);
  Tuple<int, double> copy(original);

  REQUIRE(copy.get<0>() == 42);
  REQUIRE(copy.get<1>() == 3.14);
}

TEST_CASE("Move construction", "[Core.Tuple]") {
  Tuple<std::string, int> original("hello", 42);
  Tuple<std::string, int> moved(std::move(original));

  REQUIRE(moved.get<0>() == "hello");
  REQUIRE(moved.get<1>() == 42);
}

TEST_CASE("Cross-type construction", "[Core.Tuple]") {
  Tuple<int, double> int_double(42, 3.14);
  Tuple<long, float> long_float(int_double);

  REQUIRE(long_float.get<0>() == 42L);
  REQUIRE(long_float.get<1>() == static_cast<float>(3.14));
}

TEST_CASE("Assignment operators", "[Core.Tuple]") {
  Tuple<int, double> t1(1, 1.0);
  Tuple<int, double> t2(2, 2.0);

  t1 = t2;
  REQUIRE(t1.get<0>() == 2);
  REQUIRE(t1.get<1>() == 2.0);

  Tuple<int, double> t3(3, 3.0);
  t1 = std::move(t3);
  REQUIRE(t1.get<0>() == 3);
  REQUIRE(t1.get<1>() == 3.0);
}

TEST_CASE("Cross-type assignment", "[Core.Tuple]") {
  Tuple<int, double> int_double(42, 3.14);
  Tuple<long, float> long_float(0L, 0.0f);

  long_float = int_double;
  REQUIRE(long_float.get<0>() == 42L);
  REQUIRE(long_float.get<1>() == static_cast<float>(3.14));
}

TEST_CASE("get() method - mutable access", "[Core.Tuple]") {
  Tuple<int, std::string> t(42, "hello");

  t.get<0>() = 100;
  t.get<1>() = "world";

  REQUIRE(t.get<0>() == 100);
  REQUIRE(t.get<1>() == "world");
}

TEST_CASE("get() method - const access", "[Core.Tuple]") {
  const Tuple<int, std::string> t(42, "hello");

  REQUIRE(t.get<0>() == 42);
  REQUIRE(t.get<1>() == "hello");

  // Verify const correctness
  REQUIRE(std::is_same_v<decltype(t.get<0>()), const int&>);
  REQUIRE(std::is_same_v<decltype(t.get<1>()), const std::string&>);
}

TEST_CASE("Empty tuple", "[Core.Tuple]") {
  [[maybe_unused]] Tuple<> empty;
  // Should compile and work
  REQUIRE(true);
}

TEST_CASE("Single element tuple", "[Core.Tuple]") {
  Tuple<int> single(42);
  REQUIRE(single.get<0>() == 42);
}

TEST_CASE("Complex types", "[Core.Tuple]") {
  auto ptr = std::make_unique<int>(42);
  Tuple<std::unique_ptr<int>, std::string> t(std::move(ptr), "hello");

  REQUIRE(*t.get<0>() == 42);
  REQUIRE(t.get<1>() == "hello");
}

TEST_CASE("Constexpr functionality", "[Core.Tuple]") {
  constexpr Tuple<int, double> t(42, 3.14);
  static_assert(t.get<0>() == 42);
  static_assert(t.get<1>() == 3.14);

  // Test constexpr construction detection
  static_assert(test_constexpr_construction<Tuple<int, double>>());
}

TEST_CASE("Move-only types", "[Core.Tuple]") {
  Tuple<NonCopyable, int> t(NonCopyable(42), 100);
  REQUIRE(t.get<0>().value == 42);
  REQUIRE(t.get<1>() == 100);

  // Test move construction
  Tuple<NonCopyable, int> moved(std::move(t));
  REQUIRE(moved.get<0>().value == 42);
  REQUIRE(moved.get<1>() == 100);
}

TEST_CASE("make_tuple function", "[Core.Tuple]") {
  const char* s = "hello";
  auto t = kta::make_tuple(42, 3.14, s);

  REQUIRE(std::is_same_v<decltype(t), Tuple<int, double, const char*>>);
  REQUIRE(t.get<0>() == 42);
  REQUIRE(t.get<1>() == 3.14);
  REQUIRE(std::string(t.get<2>()) == "hello");
}

TEST_CASE("make_tuple with references", "[Core.Tuple]") {
  int x = 42;
  double y = 3.14;

  auto t = kta::make_tuple(x, y);
  REQUIRE(std::is_same_v<decltype(t), Tuple<int, double>>);
  REQUIRE(t.get<0>() == 42);
  REQUIRE(t.get<1>() == 3.14);
}

TEST_CASE("make_tuple with move semantics", "[Core.Tuple]") {
  std::string s = "hello";
  auto t = kta::make_tuple(42, std::move(s));

  REQUIRE(std::is_same_v<decltype(t), Tuple<int, std::string>>);
  REQUIRE(t.template get<0>() == 42);
  REQUIRE(t.template get<1>() == "hello");
}

TEST_CASE("tuple_accessor function - const reference", "[Core.Tuple]") {
  const Tuple<int, double, std::string> t(42, 3.14, "hello");

  auto& first = kta::tuple_accessor<0>(t);
  auto& second = kta::tuple_accessor<1>(t);
  auto& third = kta::tuple_accessor<2>(t);

  REQUIRE(first == 42);
  REQUIRE(second == 3.14);
  REQUIRE(third == "hello");

  REQUIRE(std::is_same_v<decltype(first), const int&>);
  REQUIRE(std::is_same_v<decltype(second), const double&>);
  REQUIRE(std::is_same_v<decltype(third), const std::string&>);
}

TEST_CASE("tuple_accessor with complex types", "[Core.Tuple]") {
  Tuple<std::unique_ptr<int>, std::string> t(std::make_unique<int>(42), "hello");

  auto& ptr = kta::tuple_accessor<0>(t);
  auto& str = kta::tuple_accessor<1>(t);

  REQUIRE(*ptr == 42);
  REQUIRE(str == "hello");
}

TEST_CASE("kta::apply useage", "[Core.Tuple]") {
  auto functor1 = [](int x, double y, const std::string& z) -> bool {
    if(x == 33 && y == 3.14 && z == "foobar") { return true; }
    return false;
  };

  Tuple<int, double, std::string> tup1(33, 3.14, "foobar");
  const bool res1 = kta::apply(functor1, tup1);
  REQUIRE(res1);

  auto functor2 = [](bool x, const bool* y) -> bool {
    if(x == false && y != nullptr && *y == true) { return true; }
    return false;
  };

  Tuple<bool, const bool*> tup2(false, &res1);
  const bool res2 = kta::apply(functor2, tup2);
  REQUIRE(res2);
}

TEST_CASE("tuple_size specialization", "[Core.Tuple]") {
  REQUIRE(std::tuple_size_v<Tuple<int, double, std::string>> == 3);
  REQUIRE(std::tuple_size_v<Tuple<int>> == 1);
  REQUIRE(std::tuple_size_v<Tuple<>> == 0);
}

TEST_CASE("tuple_element specialization", "[Core.Tuple]") {
  using T = Tuple<int, double, std::string>;

  REQUIRE(std::is_same_v<std::tuple_element_t<0, T>, int>);
  REQUIRE(std::is_same_v<std::tuple_element_t<1, T>, double>);
  REQUIRE(std::is_same_v<std::tuple_element_t<2, T>, std::string>);
}

TEST_CASE("Structured bindings support", "[Core.Tuple]") {
  Tuple<int, double, std::string> t(42, 3.14, "hello");

  auto [a, b, c] = t;
  REQUIRE(a == 42);
  REQUIRE(b == 3.14);
  REQUIRE(c == "hello");
}

TEST_CASE("Structured bindings with references", "[Core.Tuple]") {
  Tuple<int, std::string> t(42, "hello");

  auto& [a, b] = t;
  a = 100;
  b = "world";

  REQUIRE(t.get<0>() == 100);
  REQUIRE(t.get<1>() == "world");
}

TEST_CASE("Type compatibility checks", "[Core.Tuple]") {
  // These should compile due to implicit conversions
  Tuple<double, long> t1(42, 100);
  REQUIRE(t1.get<0>() == 42.0);
  REQUIRE(t1.get<1>() == 100L);
}

TEST_CASE("Nested tuples", "[Core.Tuple]") {
  using NestedTuple = Tuple<Tuple<int, double>, std::string>;
  NestedTuple t(kta::make_tuple(42, 3.14), "hello");

  REQUIRE(t.get<0>().get<0>() == 42);
  REQUIRE(t.get<0>().get<1>() == 3.14);
  REQUIRE(t.get<1>() == "hello");
}

