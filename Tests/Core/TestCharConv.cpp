/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#define KTA_ASSUME_TESTING_ENV_
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <Kalantha/Core/CharConv.hpp>

#include <cstdint>
#include <array>
#include <cstring>
#include <string>
#include <limits>

using namespace kta;

TEST_CASE("kta::from_chars - Basic decimal conversion", "[Core.CharConv]") {
  SECTION("Positive integers") {
    int result = 0;
    auto err = kta::from_chars("123", result, BaseDec);
    REQUIRE(err.has_value());
    REQUIRE(result == 123);

    err = kta::from_chars("0", result, BaseDec);
    REQUIRE(err.has_value());
    REQUIRE(result == 0);

    err = kta::from_chars("999", result, BaseDec);
    REQUIRE(err.has_value());
    REQUIRE(result == 999);
  }

  SECTION("Negative integers") {
    int result = 0;
    auto err = kta::from_chars("-123", result, BaseDec);
    REQUIRE(err.has_value());
    REQUIRE(result == -123);

    err = kta::from_chars("-1", result, BaseDec);
    REQUIRE(err.has_value());
    REQUIRE(result == -1);
  }

  SECTION("With leading/trailing whitespace") {
    int result = 0;
    auto err = kta::from_chars("  123", result, BaseDec);
    REQUIRE(err.has_value());
    REQUIRE(result == 123);

    err = kta::from_chars("\t42\n", result, BaseDec);
    REQUIRE(err.has_value());
    REQUIRE(result == 42);
  }

  SECTION("With explicit positive sign") {
    int result = 0;
    auto err = kta::from_chars("+456", result, BaseDec);
    REQUIRE(err.has_value());
    REQUIRE(result == 456);
  }
}

TEST_CASE("kta::from_chars - Hexadecimal conversion", "[Core.CharConv]") {
  SECTION("Valid hex strings") {
    int result = 0;
    auto err = kta::from_chars("FF", result, BaseHex);
    REQUIRE(err.has_value());
    REQUIRE(result == 255);

    err = kta::from_chars("ff", result, BaseHex);
    REQUIRE(err.has_value());
    REQUIRE(result == 255);

    err = kta::from_chars("A0", result, BaseHex);
    REQUIRE(err.has_value());
    REQUIRE(result == 160);

    err = kta::from_chars("0", result, BaseHex);
    REQUIRE(err.has_value());
    REQUIRE(result == 0);
  }

  SECTION("Mixed case") {
    int result = 0;
    auto err = kta::from_chars("AbC", result, BaseHex);
    REQUIRE(err.has_value());
    REQUIRE(result == 2748); // 0xABC
  }
}

TEST_CASE("kta::from_chars - Octal conversion", "[Core.CharConv]") {
  SECTION("Valid octal strings") {
    int result = 0;
    auto err = kta::from_chars("77", result, BaseOct);
    REQUIRE(err.has_value());
    REQUIRE(result == 63); // 7*8 + 7

    err = kta::from_chars("123", result, BaseOct);
    REQUIRE(err.has_value());
    REQUIRE(result == 83); // 1*64 + 2*8 + 3

    err = kta::from_chars("0", result, BaseOct);
    REQUIRE(err.has_value());
    REQUIRE(result == 0);
  }
}

TEST_CASE("kta::from_chars - Binary conversion", "[Core.CharConv]") {
  SECTION("Valid binary strings") {
    int result = 0;
    auto err = kta::from_chars("1010", result, BaseBin);
    REQUIRE(err.has_value());
    REQUIRE(result == 10);

    err = kta::from_chars("11111111", result, BaseBin);
    REQUIRE(err.has_value());
    REQUIRE(result == 255);

    err = kta::from_chars("0", result, BaseBin);
    REQUIRE(err.has_value());
    REQUIRE(result == 0);

    err = kta::from_chars("1", result, BaseBin);
    REQUIRE(err.has_value());
    REQUIRE(result == 1);
  }
}

TEMPLATE_TEST_CASE("kta::from_chars - Different integer types", "[Core.CharConv]",
                   int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t) {
  TestType result{};
  auto err = kta::from_chars("42", result, BaseDec);
  REQUIRE(err.has_value());
  REQUIRE(result == 42);
}

TEST_CASE("kta::from_chars - Error cases", "[Core.CharConv]") {
  SECTION("Invalid arguments") {
    int result = 0;
    auto err = kta::from_chars("", result, BaseDec);
    REQUIRE(err.has_error());

    err = kta::from_chars("   ", result, BaseDec);
    REQUIRE(err.has_error());
  }

  SECTION("Invalid base") {
    int result = 0;
    auto err = kta::from_chars("123", result, 5); // Invalid base
    REQUIRE(err.has_error());
  }

  SECTION("Invalid characters for base") {
    int result = 0;
    auto err = kta::from_chars("12G", result, BaseHex); // G is invalid for hex
    REQUIRE(err.has_value()); // Should parse "12" and stop
    REQUIRE(result == 18); // 0x12

    err = kta::from_chars("128", result, BaseOct); // 8 is invalid for octal
    REQUIRE(err.has_value()); // Should parse "12" and stop
    REQUIRE(result == 10); // 012 octal

    err = kta::from_chars("102", result, BaseBin); // 2 is invalid for binary
    REQUIRE(err.has_value()); // Should parse "10" and stop
    REQUIRE(result == 2); // 0b10
  }

  SECTION("Overflow conditions") {
    int8_t small_result = 0;
    auto err = kta::from_chars("999", small_result, BaseDec); // Too large for int8_t
    REQUIRE(err.has_error());
  }
}

TEST_CASE("kta::to_chars - Basic decimal conversion", "[Core.CharConv]") {
  SECTION("Positive integers") {
    std::array<char, 32> buffer{};
    Span<char> span(buffer.data(), buffer.size());

    auto result = kta::to_chars(123, span, BaseDec);
    REQUIRE(result.has_value());
    usize len = result.value();
    std::string str(buffer.data(), len);
    REQUIRE(str == "123");

    span = Span<char>(buffer.data(), buffer.size());
    result = kta::to_chars(0, span, BaseDec);
    REQUIRE(result.has_value());
    len = result.value();
    str = std::string(buffer.data(), len);
    REQUIRE(str == "0");
  }

  SECTION("Negative integers") {
    std::array<char, 32> buffer{};
    Span<char> span(buffer.data(), buffer.size());

    auto result = kta::to_chars(-123, span, BaseDec);
    REQUIRE(result.has_value());
    usize len = result.value();
    std::string str(buffer.data(), len);
    REQUIRE(str == "-123");

    span = Span<char>(buffer.data(), buffer.size());
    result = kta::to_chars(-1, span, BaseDec);
    REQUIRE(result.has_value());
    len = result.value();
    str = std::string(buffer.data(), len);
    REQUIRE(str == "-1");
  }
}

TEST_CASE("kta::to_chars - Hexadecimal conversion", "[Core.CharConv]") {
  SECTION("Various hex values") {
    std::array<char, 32> buffer{};
    Span<char> span(buffer.data(), buffer.size());

    auto result = kta::to_chars(255, span, BaseHex);
    REQUIRE(result.has_value());
    usize len = result.value();
    std::string str(buffer.data(), len);
    REQUIRE(str == "0xFF");

    span = Span<char>(buffer.data(), buffer.size());
    result = kta::to_chars(160, span, BaseHex);
    REQUIRE(result.has_value());
    len = result.value();
    str = std::string(buffer.data(), len);
    REQUIRE(str == "0xA0");

    span = Span<char>(buffer.data(), buffer.size());
    result = kta::to_chars(0, span, BaseHex);
    REQUIRE(result.has_value());
    len = result.value();
    str = std::string(buffer.data(), len);
    REQUIRE(str == "0");
  }
}

TEST_CASE("kta::to_chars - Octal conversion", "[Core.CharConv]") {
  SECTION("Various octal values") {
    std::array<char, 32> buffer{};
    Span<char> span(buffer.data(), buffer.size());

    auto result = kta::to_chars(63, span, BaseOct); // 077 octal
    REQUIRE(result.has_value());
    usize len = result.value();
    std::string str(buffer.data(), len);
    REQUIRE(str == "077");

    span = Span<char>(buffer.data(), buffer.size());
    result = kta::to_chars(0, span, BaseOct);
    REQUIRE(result.has_value());
    len = result.value();
    str = std::string(buffer.data(), len);
    REQUIRE(str == "0");
  }
}

TEST_CASE("kta::to_chars - Binary conversion", "[Core.CharConv]") {
  SECTION("Various binary values") {
    std::array<char, 32> buffer{};
    Span<char> span(buffer.data(), buffer.size());

    auto result = kta::to_chars(10, span, BaseBin);
    REQUIRE(result.has_value());
    usize len = result.value();
    std::string str(buffer.data(), len);
    REQUIRE(str == "0b1010");

    span = Span<char>(buffer.data(), buffer.size());
    result = kta::to_chars(255, span, BaseBin);
    REQUIRE(result.has_value());
    len = result.value();
    str = std::string(buffer.data(), len);
    REQUIRE(str == "0b11111111");

    span = Span<char>(buffer.data(), buffer.size());
    result = kta::to_chars(0, span, BaseBin);
    REQUIRE(result.has_value());
    len = result.value();
    str = std::string(buffer.data(), len);
    REQUIRE(str == "0");
  }
}

TEMPLATE_TEST_CASE("kta::to_chars - Different integer types", "[Core.CharConv]",
                   int8_t, uint8_t, int16_t, uint16_t, int32_t, uint32_t, int64_t, uint64_t) {
  std::array<char, 64> buffer{};
  Span<char> span(buffer.data(), buffer.size());

  TestType value = 42;
  auto result = kta::to_chars(value, span, BaseDec);
  REQUIRE(result.has_value());
  usize len = result.value();
  std::string str(buffer.data(), len);
  REQUIRE(str == "42");
}

TEST_CASE("kta::to_chars - Error cases", "[Core.CharConv]") {
  SECTION("Buffer too small") {
    std::array<char, 2> small_buffer{};
    Span<char> span(small_buffer.data(), small_buffer.size());

    auto result = kta::to_chars(12345, span, BaseDec); // Needs more than 2 chars
    REQUIRE(result.has_error());
  }

  SECTION("Invalid base") {
    std::array<char, 32> buffer{};
    Span<char> span(buffer.data(), buffer.size());

    auto result = kta::to_chars(123, span, 5); // Invalid base
    REQUIRE(result.has_error());
  }
}

TEST_CASE("Round-trip conversion tests", "[Core.CharConv]") {
  SECTION("Decimal round-trip") {
    std::array<char, 32> buffer{};
    Span<char> span(buffer.data(), buffer.size());

    int original = 12345;
    auto to_result = kta::to_chars(original, span, BaseDec);
    REQUIRE(to_result.has_value());

    usize len = to_result.value();
    StringView sv(buffer.data(), len);

    int parsed = 0;
    auto from_result = kta::from_chars(sv, parsed, BaseDec);
    REQUIRE(from_result.has_value());
    REQUIRE(parsed == original);
  }

  SECTION("Hexadecimal round-trip") {
    std::array<char, 32> buffer{};
    Span<char> span(buffer.data(), buffer.size());

    int original = 0xABCD;
    auto to_result = kta::to_chars(original, span, BaseHex);
    REQUIRE(to_result.has_value());

    usize len = to_result.value();
    // Skip the "0x" prefix for parsing
    StringView sv(buffer.data() + 2, len - 2);

    int parsed = 0;
    auto from_result = kta::from_chars(sv, parsed, BaseHex);
    REQUIRE(from_result.has_value());
    REQUIRE(parsed == original);
  }

  SECTION("Binary round-trip") {
    std::array<char, 32> buffer{};
    Span<char> span(buffer.data(), buffer.size());

    int original = 0b101010;
    auto to_result = kta::to_chars(original, span, BaseBin);
    REQUIRE(to_result.has_value());

    usize len = to_result.value();
    // Skip the "0b" prefix for parsing
    StringView sv(buffer.data() + 2, len - 2);

    int parsed = 0;
    auto from_result = kta::from_chars(sv, parsed, BaseBin);
    REQUIRE(from_result.has_value());
    REQUIRE(parsed == original);
  }

  SECTION("Octal round-trip") {
    std::array<char, 32> buffer{};
    Span<char> span(buffer.data(), buffer.size());

    int original = 0755; // Octal literal
    auto to_result = kta::to_chars(original, span, BaseOct);
    REQUIRE(to_result.has_value());

    usize len = to_result.value();
    // Skip the "0" prefix for parsing
    StringView sv(buffer.data() + 1, len - 1);

    int parsed = 0;
    auto from_result = kta::from_chars(sv, parsed, BaseOct);
    REQUIRE(from_result.has_value());
    REQUIRE(parsed == original);
  }
}

TEST_CASE("Edge cases and limits", "[Core.CharConv]") {
  SECTION("Maximum values") {
    std::array<char, 64> buffer{};
    Span<char> span(buffer.data(), buffer.size());

    // Test with maximum int32_t value
    int32_t max_val = std::numeric_limits<int32_t>::max();
    auto result = kta::to_chars(max_val, span, BaseDec);
    REQUIRE(result.has_value());

    usize len = result.value();
    StringView sv(buffer.data(), len);

    int32_t parsed = 0;
    auto from_result = kta::from_chars(sv, parsed, BaseDec);
    REQUIRE(from_result.has_value());
    REQUIRE(parsed == max_val);
  }
}
