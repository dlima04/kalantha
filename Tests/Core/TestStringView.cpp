/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#define KTA_ASSUME_TESTING_ENV_
#include <catch2/catch_test_macros.hpp>
#include <Kalantha/Core/StringView.hpp>
#include <string>

using namespace kta;
using namespace kta::string_literals;

TEST_CASE("StringView construction", "[Core.StringView]") {
  SECTION("Default construction") {
    StringView sv;
    REQUIRE(sv.data() == nullptr);
    REQUIRE(sv.size() == 0);
    REQUIRE(sv.empty());
    REQUIRE(sv.begin() == sv.end());
  }

  SECTION("Construction from null-terminated string") {
    const char* str = "hello";
    StringView sv(str);
    REQUIRE(sv.data() == str);
    REQUIRE(sv.size() == 5);
    REQUIRE(!sv.empty());
    REQUIRE(sv[0] == 'h');
    REQUIRE(sv[4] == 'o');
  }

  SECTION("Construction from empty string") {
    const char* str = "";
    StringView sv(str);
    REQUIRE(sv.data() == str);
    REQUIRE(sv.size() == 0);
    REQUIRE(sv.empty());
  }

  SECTION("Construction from pointer and length") {
    const char* str = "hello world";
    StringView sv(str, 5);
    REQUIRE(sv.data() == str);
    REQUIRE(sv.size() == 5);
    REQUIRE(!sv.empty());
    REQUIRE(sv[0] == 'h');
    REQUIRE(sv[4] == 'o');
  }

  SECTION("Construction from pointer and zero length") {
    const char* str = "hello";
    StringView sv(str, 0);
    REQUIRE(sv.data() == str);
    REQUIRE(sv.size() == 0);
    REQUIRE(sv.empty());
  }

  SECTION("Construction from character array") {
    const char arr[] = "test";
    StringView sv(arr);
    REQUIRE(sv.data() == arr);
    REQUIRE(sv.size() == 4); // Should be 4, not 5 (excluding null terminator)
    REQUIRE(!sv.empty());
    REQUIRE(sv[0] == 't');
    REQUIRE(sv[3] == 't');
  }

  SECTION("Construction from single character array") {
    const char arr[] = "x";
    StringView sv(arr);
    REQUIRE(sv.data() == arr);
    REQUIRE(sv.size() == 1);
    REQUIRE(!sv.empty());
    REQUIRE(sv[0] == 'x');
  }

  SECTION("Copy construction") {
    const char* str = "hello";
    StringView original(str);
    StringView copy(original);
    REQUIRE(copy.data() == original.data());
    REQUIRE(copy.size() == original.size());
    REQUIRE(copy[0] == 'h');
    REQUIRE(copy[4] == 'o');
  }

  SECTION("Move construction") {
    const char* str = "hello";
    StringView original(str);
    StringView moved(std::move(original));
    REQUIRE(moved.data() == str);
    REQUIRE(moved.size() == 5);
    REQUIRE(moved[0] == 'h');
  }
}

TEST_CASE("StringView character types", "[Core.StringView]") {
  SECTION("char type") {
    const char* str = "hello";
    StringView sv(str);
    REQUIRE(sv.size() == 5);
    REQUIRE(sv[0] == 'h');
  }

  SECTION("char8_t type") {
    const char8_t* str = u8"hello";
    U8StringView sv(str);
    REQUIRE(sv.size() == 5);
    REQUIRE(sv[0] == u8'h');
  }

  SECTION("wchar_t type") {
    const wchar_t* str = L"hello";
    WStringView sv(str);
    REQUIRE(sv.size() == 5);
    REQUIRE(sv[0] == L'h');
  }
}

TEST_CASE("StringView element access", "[Core.StringView]") {
  const char* str = "hello world";
  StringView sv(str);

  SECTION("Operator[] access") {
    REQUIRE(sv[0] == 'h');
    REQUIRE(sv[4] == 'o');
    REQUIRE(sv[5] == ' ');
    REQUIRE(sv[6] == 'w');
    REQUIRE(sv[10] == 'd');
  }

  SECTION("at() method") {
    REQUIRE(sv.at(0) == 'h');
    REQUIRE(sv.at(4) == 'o');
    REQUIRE(sv.at(5) == ' ');
    REQUIRE(sv.at(6) == 'w');
    REQUIRE(sv.at(10) == 'd');
  }

  SECTION("const correctness") {
    const StringView const_sv(str);
    REQUIRE(const_sv[0] == 'h');
    REQUIRE(const_sv.at(0) == 'h');
  }
}

TEST_CASE("StringView properties", "[Core.StringView]") {
  SECTION("size() method") {
    StringView sv1("hello");
    REQUIRE(sv1.size() == 5);
    
    StringView sv2("a");
    REQUIRE(sv2.size() == 1);
    
    StringView sv3("");
    REQUIRE(sv3.size() == 0);
    
    StringView sv4("hello", 3);
    REQUIRE(sv4.size() == 3);
  }

  SECTION("size_bytes() method") {
    StringView sv1("hello");
    REQUIRE(sv1.size_bytes() == 5 * sizeof(char));
    
    WStringView sv2(L"hello");
    REQUIRE(sv2.size_bytes() == 5 * sizeof(wchar_t));
    
    StringView sv3("");
    REQUIRE(sv3.size_bytes() == 0);
    
    StringView sv4("test", 2);
    REQUIRE(sv4.size_bytes() == 2 * sizeof(char));
  }

  SECTION("empty() method") {
    StringView empty_sv;
    REQUIRE(empty_sv.empty());
    
    StringView empty_str("");
    REQUIRE(empty_str.empty());
    
    StringView non_empty("hello");
    REQUIRE(!non_empty.empty());
    
    StringView zero_length("hello", 0);
    REQUIRE(zero_length.empty());
  }

  SECTION("data() method") {
    const char* str = "hello";
    StringView sv(str);
    REQUIRE(sv.data() == str);
    
    StringView empty_sv;
    REQUIRE(empty_sv.data() == nullptr);
  }
}

TEST_CASE("StringView iterators", "[Core.StringView]") {
  const char* str = "hello";
  StringView sv(str);

  SECTION("begin() and end()") {
    auto it = sv.begin();
    REQUIRE(*it == 'h');
    ++it;
    REQUIRE(*it == 'e');
    
    auto end_it = sv.end();
    REQUIRE(end_it - sv.begin() == 5);
  }

  SECTION("Range-based for loop") {
    const char expected[] = "hello";
    int i = 0;
    for (const auto& ch : sv) {
      REQUIRE(ch == expected[i++]);
    }
    REQUIRE(i == 5);
  }

  SECTION("Iterator traversal") {
    std::string result;
    for (auto it = sv.begin(); it != sv.end(); ++it) {
      result += *it;
    }
    REQUIRE(result == "hello");
  }

  SECTION("Empty string iterators") {
    StringView empty_sv("");
    REQUIRE(empty_sv.begin() == empty_sv.end());
    
    int count = 0;
    for (auto ch : empty_sv) {
      (void)ch; // Suppress unused variable warning
      ++count;
    }
    REQUIRE(count == 0);
  }

  SECTION("Const iterators") {
    const StringView const_sv(str);
    auto it = const_sv.begin();
    REQUIRE(*it == 'h');
    REQUIRE(const_sv.end() - const_sv.begin() == 5);
  }
}

TEST_CASE("StringView comparison operators", "[Core.StringView]") {
  SECTION("Equality operator") {
    StringView sv1("hello");
    StringView sv2("hello");
    StringView sv3("world");
    StringView sv4("hello!");
    StringView sv5("hell");
    
    REQUIRE(sv1 == sv2);
    REQUIRE(!(sv1 == sv3));
    REQUIRE(!(sv1 == sv4));
    REQUIRE(!(sv1 == sv5));
  }

  SECTION("Inequality operator") {
    StringView sv1("hello");
    StringView sv2("hello");
    StringView sv3("world");
    StringView sv4("hello!");
    StringView sv5("hell");
    
    REQUIRE(!(sv1 != sv2));
    REQUIRE(sv1 != sv3);
    REQUIRE(sv1 != sv4);
    REQUIRE(sv1 != sv5);
  }

  SECTION("Empty string comparisons") {
    StringView empty1("");
    StringView empty2;
    StringView empty3("", 0);
    StringView non_empty("a");
    
    REQUIRE(empty1 == empty2);
    REQUIRE(empty1 == empty3);
    REQUIRE(empty1 != non_empty);
  }

  SECTION("Same content, different sources") {
    const char* str1 = "hello";
    const char str2[] = "hello";
    StringView sv1(str1);
    StringView sv2(str2);
    
    REQUIRE(sv1 == sv2);
    REQUIRE(!(sv1 != sv2));
  }

  SECTION("Different character types") {
    StringView sv1("hello");
    const char arr[] = "hello";
    StringView sv2(arr);
    
    REQUIRE(sv1 == sv2);
  }

  SECTION("Partial string comparisons") {
    StringView sv1("hello world", 5);
    StringView sv2("hello");
    StringView sv3("hello universe", 5);
    
    REQUIRE(sv1 == sv2);
    REQUIRE(sv1 == sv3);
    REQUIRE(sv2 == sv3);
  }
}

TEST_CASE("StringView subview operations", "[Core.StringView]") {
  const char* str = "hello world";
  StringView sv(str);

  SECTION("subview with offset and count") {
    auto sub = sv.subview(6, 5);
    REQUIRE(sub.size() == 5);
    REQUIRE(sub[0] == 'w');
    REQUIRE(sub[4] == 'd');
    REQUIRE(sub.data() == str + 6);
    
    // Verify it's a proper substring
    StringView expected("world");
    REQUIRE(sub == expected);
  }

  SECTION("subview with offset only") {
    auto sub = sv.subview(6);
    REQUIRE(sub.size() == 5);
    REQUIRE(sub[0] == 'w');
    REQUIRE(sub[4] == 'd');
    REQUIRE(sub.data() == str + 6);
    
    StringView expected("world");
    REQUIRE(sub == expected);
  }

  SECTION("subview at beginning") {
    auto sub = sv.subview(0, 5);
    REQUIRE(sub.size() == 5);
    REQUIRE(sub[0] == 'h');
    REQUIRE(sub[4] == 'o');
    REQUIRE(sub.data() == str);
    
    StringView expected("hello");
    REQUIRE(sub == expected);
  }

  SECTION("subview at end") {
    auto sub = sv.subview(6, 5);
    REQUIRE(sub.size() == 5);
    REQUIRE(sub[0] == 'w');
    REQUIRE(sub[4] == 'd');
    
    StringView expected("world");
    REQUIRE(sub == expected);
  }

  SECTION("subview entire string") {
    auto sub = sv.subview(0, sv.size());
    REQUIRE(sub.size() == sv.size());
    REQUIRE(sub.data() == sv.data());
    REQUIRE(sub == sv);
  }

  SECTION("subview empty") {
    auto sub = sv.subview(5, 0);
    REQUIRE(sub.size() == 0);
    REQUIRE(sub.empty());
    REQUIRE(sub.data() == str + 5);
  }

  SECTION("subview single character") {
    auto sub = sv.subview(0, 1);
    REQUIRE(sub.size() == 1);
    REQUIRE(sub[0] == 'h');
    
    StringView expected("h");
    REQUIRE(sub == expected);
  }

  SECTION("subview from end") {
    auto sub = sv.subview(sv.size());
    REQUIRE(sub.size() == 0);
    REQUIRE(sub.empty());
  }

  SECTION("Chained subviews") {
    auto sub1 = sv.subview(0, 8); // "hello wo"
    auto sub2 = sub1.subview(6, 2); // "wo"
    REQUIRE(sub2.size() == 2);
    REQUIRE(sub2[0] == 'w');
    REQUIRE(sub2[1] == 'o');
    
    StringView expected("wo");
    REQUIRE(sub2 == expected);
  }
}

TEST_CASE("StringView string literals", "[Core.StringView]") {
  SECTION("char string literal") {
    auto sv = "hello world"_sv;
    REQUIRE(sv.size() == 11);
    REQUIRE(sv[0] == 'h');
    REQUIRE(sv[10] == 'd');
    
    StringView expected("hello world");
    REQUIRE(sv == expected);
  }

  SECTION("char8_t string literal") {
    auto sv = u8"hello world"_sv;
    REQUIRE(sv.size() == 11);
    REQUIRE(sv[0] == u8'h');
    REQUIRE(sv[10] == u8'd');
  }

  SECTION("wchar_t string literal") {
    auto sv = L"hello world"_sv;
    REQUIRE(sv.size() == 11);
    REQUIRE(sv[0] == L'h');
    REQUIRE(sv[10] == L'd');
  }

  SECTION("Empty string literal") {
    auto sv = ""_sv;
    REQUIRE(sv.size() == 0);
    REQUIRE(sv.empty());
  }
}

TEST_CASE("StringView edge cases", "[Core.StringView]") {
  SECTION("Single character string") {
    StringView sv("x");
    REQUIRE(sv.size() == 1);
    REQUIRE(sv[0] == 'x');
    REQUIRE(!sv.empty());
    
    auto sub = sv.subview(0, 1);
    REQUIRE(sub == sv);
  }

  SECTION("String with null characters") {
    const char str[] = {'h', 'e', 'l', '\0', 'l', 'o'};
    StringView sv(str, 6);
    REQUIRE(sv.size() == 6);
    REQUIRE(sv[0] == 'h');
    REQUIRE(sv[3] == '\0');
    REQUIRE(sv[5] == 'o');
  }

  SECTION("Overlapping subviews") {
    StringView sv("hello world");
    auto sub1 = sv.subview(0, 7);  // "hello w"
    auto sub2 = sv.subview(4, 7);  // "o world"
    
    REQUIRE(sub1.size() == 7);
    REQUIRE(sub2.size() == 7);
    REQUIRE(sub1[4] == 'o');
    REQUIRE(sub2[0] == 'o');
    REQUIRE(sub1 != sub2);
  }

  SECTION("Comparison with different lengths") {
    StringView sv1("hello");
    StringView sv2("hello world");
    StringView sv3("hell");
    
    REQUIRE(sv1 != sv2);
    REQUIRE(sv1 != sv3);
    REQUIRE(sv2 != sv3);
  }
}

