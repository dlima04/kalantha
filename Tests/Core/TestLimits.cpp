/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#define KTA_ASSUME_TESTING_ENV_
#include <catch2/catch_test_macros.hpp>
#include <Kalantha/Core/Limits.hpp>

#include <limits>
#include <cstdint>

using namespace kta;

TEST_CASE("NumericLimits<int64> matches std::numeric_limits<int64>", "[Core.Limits]") {
  SECTION("max() values match") {
    REQUIRE(NumericLimits<int64>::max() == std::numeric_limits<int64>::max());
  }

  SECTION("min() values match") {
    REQUIRE(NumericLimits<int64>::min() == std::numeric_limits<int64>::min());
  }

  SECTION("is_signed matches") {
    REQUIRE(NumericLimits<int64>::is_signed == std::numeric_limits<int64>::is_signed);
  }
}

TEST_CASE("NumericLimits<uint64> matches std::numeric_limits<uint64>", "[Core.Limits]") {
  SECTION("max() values match") {
    REQUIRE(NumericLimits<uint64>::max() == std::numeric_limits<uint64>::max());
  }

  SECTION("min() values match") {
    REQUIRE(NumericLimits<uint64>::min() == std::numeric_limits<uint64>::min());
  }

  SECTION("is_signed matches") {
    REQUIRE(NumericLimits<uint64>::is_signed == std::numeric_limits<uint64>::is_signed);
  }
}

TEST_CASE("NumericLimits<int32> matches std::numeric_limits<int32>", "[Core.Limits]") {
  SECTION("max() values match") {
    REQUIRE(NumericLimits<int32>::max() == std::numeric_limits<int32>::max());
  }

  SECTION("min() values match") {
    REQUIRE(NumericLimits<int32>::min() == std::numeric_limits<int32>::min());
  }

  SECTION("is_signed matches") {
    REQUIRE(NumericLimits<int32>::is_signed == std::numeric_limits<int32>::is_signed);
  }
}

TEST_CASE("NumericLimits<uint32> matches std::numeric_limits<uint32>", "[Core.Limits]") {
  SECTION("max() values match") {
    REQUIRE(NumericLimits<uint32>::max() == std::numeric_limits<uint32>::max());
  }

  SECTION("min() values match") {
    REQUIRE(NumericLimits<uint32>::min() == std::numeric_limits<uint32>::min());
  }

  SECTION("is_signed matches") {
    REQUIRE(NumericLimits<uint32>::is_signed == std::numeric_limits<uint32>::is_signed);
  }
}

TEST_CASE("NumericLimits<int16> matches std::numeric_limits<int16>", "[Core.Limits]") {
  SECTION("max() values match") {
    REQUIRE(NumericLimits<int16>::max() == std::numeric_limits<int16>::max());
  }

  SECTION("min() values match") {
    REQUIRE(NumericLimits<int16>::min() == std::numeric_limits<int16>::min());
  }

  SECTION("is_signed matches") {
    REQUIRE(NumericLimits<int16>::is_signed == std::numeric_limits<int16>::is_signed);
  }
}

TEST_CASE("NumericLimits<uint16> matches std::numeric_limits<uint16>", "[Core.Limits]") {
  SECTION("max() values match") {
    REQUIRE(NumericLimits<uint16>::max() == std::numeric_limits<uint16>::max());
  }

  SECTION("min() values match") {
    REQUIRE(NumericLimits<uint16>::min() == std::numeric_limits<uint16>::min());
  }

  SECTION("is_signed matches") {
    REQUIRE(NumericLimits<uint16>::is_signed == std::numeric_limits<uint16>::is_signed);
  }
}

TEST_CASE("NumericLimits<int8> matches std::numeric_limits<int8>", "[Core.Limits]") {
  SECTION("max() values match") {
    REQUIRE(NumericLimits<int8>::max() == std::numeric_limits<int8>::max());
  }

  SECTION("min() values match") {
    REQUIRE(NumericLimits<int8>::min() == std::numeric_limits<int8>::min());
  }

  SECTION("is_signed matches") {
    REQUIRE(NumericLimits<int8>::is_signed == std::numeric_limits<int8>::is_signed);
  }
}

TEST_CASE("NumericLimits<uint8> matches std::numeric_limits<uint8>", "[Core.Limits]") {
  SECTION("max() values match") {
    REQUIRE(NumericLimits<uint8>::max() == std::numeric_limits<uint8>::max());
  }

  SECTION("min() values match") {
    REQUIRE(NumericLimits<uint8>::min() == std::numeric_limits<uint8>::min());
  }

  SECTION("is_signed matches") {
    REQUIRE(NumericLimits<uint8>::is_signed == std::numeric_limits<uint8>::is_signed);
  }
}

TEST_CASE("NumericLimits<char8_t> matches std::numeric_limits<char8_t>", "[Core.Limits]") {
  SECTION("max() values match") {
    REQUIRE(NumericLimits<char8_t>::max() == std::numeric_limits<char8_t>::max());
  }

  SECTION("min() values match") {
    REQUIRE(NumericLimits<char8_t>::min() == std::numeric_limits<char8_t>::min());
  }

  SECTION("is_signed matches") {
    REQUIRE(NumericLimits<char8_t>::is_signed == std::numeric_limits<char8_t>::is_signed);
  }
}

TEST_CASE("NumericLimits<bool> matches std::numeric_limits<bool>", "[Core.Limits]") {
  SECTION("max() values match") {
    REQUIRE(NumericLimits<bool>::max() == std::numeric_limits<bool>::max());
  }

  SECTION("min() values match") {
    REQUIRE(NumericLimits<bool>::min() == std::numeric_limits<bool>::min());
  }

  SECTION("is_signed matches") {
    REQUIRE(NumericLimits<bool>::is_signed == std::numeric_limits<bool>::is_signed);
  }
}
