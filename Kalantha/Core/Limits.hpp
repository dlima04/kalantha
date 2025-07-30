/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>
#include <Kalantha/Core/Types.hpp>
#include <Kalantha/Meta/TypeTraits.hpp>
BEGIN_NAMESPACE_KTA_

template<typename T>
struct NumericLimits {
  static_assert(FalseType<T>::value, "Invalid type.");
};

/// NOTE:
/// for any given signed integer type with n bits,
/// its minimum will be -2^{n - 1} (assuming a two's complement
/// representation). Negating its maximum and subtracting 1 is a
/// cleaner way to get this value.

template<>
struct NumericLimits<int64> {
  constexpr static int64 max() { return __INT64_MAX__; }
  constexpr static int64 min() { return -__INT64_MAX__ - 1ll; }
  constexpr static bool is_signed = true;
};

template<>
struct NumericLimits<uint64> {
  constexpr static uint64 max() { return __UINT64_MAX__; }
  constexpr static uint64 min() { return 0ULL; }
  constexpr static bool is_signed = false;
};

template<>
struct NumericLimits<int32> {
  constexpr static int32 max() { return __INT32_MAX__; }
  constexpr static int32 min() { return -__INT32_MAX__ - 1; }
  constexpr static bool is_signed = true;
};

template<>
struct NumericLimits<uint32> {
  constexpr static uint32 max() { return __UINT32_MAX__; }
  constexpr static uint32 min() { return 0; }
  constexpr static bool is_signed = false;
};

template<>
struct NumericLimits<int16> {
  constexpr static int16 max() { return __INT16_MAX__; }
  constexpr static int16 min() { return -__INT16_MAX__ - 1; }
  constexpr static bool is_signed = true;
};

template<>
struct NumericLimits<uint16> {
  constexpr static uint16 max() { return __UINT16_MAX__; }
  constexpr static uint16 min() { return 0; }
  constexpr static bool is_signed = false;
};

template<>
struct NumericLimits<int8> {
  constexpr static int8 max() { return __INT8_MAX__; }
  constexpr static int8 min() { return -__INT8_MAX__ - 1; }
  constexpr static bool is_signed = true;
};

template<>
struct NumericLimits<uint8> {
  constexpr static uint8 max() { return __UINT8_MAX__; }
  constexpr static uint8 min() { return 0; }
  constexpr static bool is_signed = false;
};

template<>
struct NumericLimits<char8_t> {
  constexpr static char8_t max() { return static_cast<char8_t>(__UINT8_MAX__); }
  constexpr static char8_t min() { return 0; }
  constexpr static bool is_signed = false;
};

template<>
struct NumericLimits<bool> {
  constexpr static bool max() { return true; }
  constexpr static bool min() { return false; }
  constexpr static bool is_signed = false;
};

END_NAMESPACE_KTA_
