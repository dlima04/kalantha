/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#define KTA_ASSUME_TESTING_ENV_
#include <catch2/catch_test_macros.hpp>
#include <Kalantha/Core/Algorithm.hpp>

using namespace kta;

TEST_CASE("min returns the smaller value", "[Core.Algorithm]") {
  REQUIRE(::kta::min(5, 10) == 5);
  REQUIRE(::kta::min(10, 5) == 5);
}

TEST_CASE("max returns the larger value", "[Core.Algorithm]") {
  REQUIRE(::kta::max(5, 10) == 10);
  REQUIRE(::kta::max(10, 5) == 10);
}

TEST_CASE("clamp limits the value within bounds", "[Core.Algorithm]") {
  REQUIRE(::kta::clamp(5, 0, 10) == 5);
  REQUIRE(::kta::clamp(-5, 0, 10) == 0);
  REQUIRE(::kta::clamp(15, 0, 10) == 10);
}
