/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#define KTA_ASSUME_TESTING_ENV_
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <Kalantha/Arch/Generic/Endian.hpp>

#include <cstring>
#include <array>

using namespace kta;

TEST_CASE("Endian detection", "[Core.Endian]") {
  SECTION("Endian enum values") {
    REQUIRE(static_cast<int>(Endian::Little) == 0);
    REQUIRE(static_cast<int>(Endian::Big) == 1);

    // Native should be either Little or Big
    REQUIRE((Endian::Native == Endian::Little || Endian::Native == Endian::Big));
  }

  SECTION("Endian detection functions") {
    REQUIRE(is_little_endian() == (Endian::Native == Endian::Little));
    REQUIRE(is_big_endian() == (Endian::Native == Endian::Big));

    // Exactly one should be true
    REQUIRE(is_little_endian() != is_big_endian());
  }
}

TEST_CASE("Byte swapping - single byte", "[Core.Endian]") {
  SECTION("8-bit values don't change") {
    uint8 val = 0x42;
    REQUIRE(byteswap(val) == val);

    int8 signed_val = -42;
    REQUIRE(byteswap(signed_val) == signed_val);

    char char_val = 'A';
    REQUIRE(byteswap(char_val) == char_val);
  }
}

TEST_CASE("Byte swapping - 16-bit", "[Core.Endian]") {
  SECTION("uint16 byte swap") {
    REQUIRE(byteswap16(0x1234) == 0x3412);
    REQUIRE(byteswap16(0x0000) == 0x0000);
    REQUIRE(byteswap16(0xFFFF) == 0xFFFF);
    REQUIRE(byteswap16(0xFF00) == 0x00FF);
    REQUIRE(byteswap16(0x00FF) == 0xFF00);
  }

  SECTION("Generic 16-bit byte swap") {
    uint16 val = 0x1234;
    REQUIRE(byteswap(val) == 0x3412);

    int16 signed_val = 0x1234;
    REQUIRE(byteswap(signed_val) == static_cast<int16>(0x3412));

    // Test double swap returns original
    REQUIRE(byteswap(byteswap(val)) == val);
  }
}

TEST_CASE("Byte swapping - 32-bit", "[Core.Endian]") {
  SECTION("uint32 byte swap") {
    REQUIRE(byteswap32(0x12345678) == 0x78563412);
    REQUIRE(byteswap32(0x00000000) == 0x00000000);
    REQUIRE(byteswap32(0xFFFFFFFF) == 0xFFFFFFFF);
    REQUIRE(byteswap32(0xFF000000) == 0x000000FF);
    REQUIRE(byteswap32(0x000000FF) == 0xFF000000);
  }

  SECTION("Generic 32-bit byte swap") {
    uint32 val = 0x12345678;
    REQUIRE(byteswap(val) == 0x78563412);

    int32 signed_val = 0x12345678;
    REQUIRE(byteswap(signed_val) == static_cast<int32>(0x78563412));

    // Test double swap returns original
    REQUIRE(byteswap(byteswap(val)) == val);
  }
}

TEST_CASE("Byte swapping - 64-bit", "[Core.Endian]") {
  SECTION("uint64 byte swap") {
    REQUIRE(byteswap64(0x123456789ABCDEF0ULL) == 0xF0DEBC9A78563412ULL);
    REQUIRE(byteswap64(0x0000000000000000ULL) == 0x0000000000000000ULL);
    REQUIRE(byteswap64(0xFFFFFFFFFFFFFFFFULL) == 0xFFFFFFFFFFFFFFFFULL);
    REQUIRE(byteswap64(0xFF00000000000000ULL) == 0x00000000000000FFULL);
    REQUIRE(byteswap64(0x00000000000000FFULL) == 0xFF00000000000000ULL);
  }

  SECTION("Generic 64-bit byte swap") {
    uint64 val = 0x123456789ABCDEF0ULL;
    REQUIRE(byteswap(val) == 0xF0DEBC9A78563412ULL);

    int64 signed_val = 0x123456789ABCDEF0LL;
    REQUIRE(byteswap(signed_val) == static_cast<int64>(0xF0DEBC9A78563412ULL));

    // Test double swap returns original
    REQUIRE(byteswap(byteswap(val)) == val);
  }
}

TEMPLATE_TEST_CASE("Byte swap idempotency", "[Core.Endian]",
                   uint16, int16, uint32, int32, uint64, int64) {
  SECTION("Double swap returns original") {
    TestType val = static_cast<TestType>(0x123456789ABCDEF0ULL);
    REQUIRE(byteswap(byteswap(val)) == val);
  }
}

TEST_CASE("Host to endian conversions", "[Core.Endian]") {
  SECTION("host_to_big and big_to_host") {
    uint32 val = 0x12345678;
    uint32 big_val = host_to_big(val);

    if (is_little_endian()) {
      REQUIRE(big_val == 0x78563412);
    } else {
      REQUIRE(big_val == val);
    }

    REQUIRE(big_to_host(big_val) == val);
  }

  SECTION("host_to_little and little_to_host") {
    uint32 val = 0x12345678;
    uint32 little_val = host_to_little(val);

    if (is_big_endian()) {
      REQUIRE(little_val == 0x78563412);
    } else {
      REQUIRE(little_val == val);
    }

    REQUIRE(little_to_host(little_val) == val);
  }
}

TEMPLATE_TEST_CASE("Endian conversion roundtrip", "[Core.Endian]",
                   uint16, int16, uint32, int32, uint64, int64) {
  TestType val = static_cast<TestType>(0x123456789ABCDEF0ULL);

  SECTION("Big endian roundtrip") {
    REQUIRE(big_to_host(host_to_big(val)) == val);
  }

  SECTION("Little endian roundtrip") {
    REQUIRE(little_to_host(host_to_little(val)) == val);
  }
}

TEST_CASE("Edge cases and corner values", "[Core.Endian]") {
  SECTION("Zero values") {
    REQUIRE(byteswap(uint16{0}) == 0);
    REQUIRE(byteswap(uint32{0}) == 0);
    REQUIRE(byteswap(uint64{0}) == 0);
  }

  SECTION("Maximum values") {
    REQUIRE(byteswap(uint16{0xFFFF}) == 0xFFFF);
    REQUIRE(byteswap(uint32{0xFFFFFFFF}) == 0xFFFFFFFF);
    REQUIRE(byteswap(uint64{0xFFFFFFFFFFFFFFFFULL}) == 0xFFFFFFFFFFFFFFFFULL);
  }

  SECTION("Power of 2 values") {
    REQUIRE(byteswap(uint16{0x0100}) == 0x0001);
    REQUIRE(byteswap(uint32{0x01000000}) == 0x00000001);
    REQUIRE(byteswap(uint64{0x0100000000000000ULL}) == 0x0000000000000001ULL);
  }
}

TEST_CASE("Signed integer handling", "[Core.Endian]") {
  SECTION("Negative values") {
    int16 neg16 = -1234;
    int16 swapped16 = byteswap(neg16);
    REQUIRE(byteswap(swapped16) == neg16);

    int32 neg32 = -123456;
    int32 swapped32 = byteswap(neg32);
    REQUIRE(byteswap(swapped32) == neg32);

    int64 neg64 = -123456789LL;
    int64 swapped64 = byteswap(neg64);
    REQUIRE(byteswap(swapped64) == neg64);
  }
}

TEST_CASE("Constexpr evaluation", "[Core.Endian]") {
  SECTION("Compile-time evaluation") {
    constexpr uint16 val16 = 0x1234;
    constexpr uint16 swapped16 = byteswap(val16);
    static_assert(swapped16 == 0x3412);

    constexpr uint32 val32 = 0x12345678;
    constexpr uint32 swapped32 = byteswap(val32);
    static_assert(swapped32 == 0x78563412);

    constexpr uint64 val64 = 0x123456789ABCDEF0ULL;
    constexpr uint64 swapped64 = byteswap(val64);
    static_assert(swapped64 == 0xF0DEBC9A78563412ULL);
    REQUIRE_FALSE(false); //lolol
  }

  SECTION("Constexpr endian detection") {
    constexpr bool little = is_little_endian();
    constexpr bool big = is_big_endian();
    static_assert(little != big);
    REQUIRE_FALSE(false);
  }
}
