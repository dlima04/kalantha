/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>
#include <Kalantha/Core/Platform.hpp>
#include <Kalantha/Core/Types.hpp>
#include <Kalantha/Meta/TypeTraits.hpp>
#include <Kalantha/Meta/Concepts.hpp>
BEGIN_NAMESPACE_KTA_

/// Note: we assume these builtins are constexpr-compatible.
#  ifdef __has_builtin
#  if __has_builtin(__builtin_bswap64)
#define KTA_HAS_BUILTIN_BSWAP64_
#  endif
#  endif

#  ifdef __has_builtin
#  if __has_builtin(__builtin_bswap32)
#define KTA_HAS_BUILTIN_BSWAP32_
#  endif
#  endif

#  ifdef __has_builtin
#  if __has_builtin(__builtin_bswap16)
#define KTA_HAS_BUILTIN_BSWAP16_
#  endif
#  endif

/// Most reliable method of detection. Should work for most GCC & Clang versions.
#  if defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && defined(__ORDER_BIG_ENDIAN__)
#  if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define KTA_ARCH_IS_LITTLE_ENDIAN_
#  elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#define KTA_ARCH_IS_BIG_ENDIAN_
#  endif
#  endif

/// Potential fallback method. Not reliable.
#  if defined(__LITTLE_ENDIAN__) && !defined(KTA_ARCH_IS_LITTLE_ENDIAN_)
#define KTA_ARCH_IS_LITTLE_ENDIAN_
#  elif defined(__BIG_ENDIAN__) && !defined(KTA_ARCH_IS_BIG_ENDIAN_)
#define KTA_ARCH_IS_BIG_ENDIAN_
#  endif

#  if !defined(KTA_ARCH_IS_LITTLE_ENDIAN_) && !defined(KTA_ARCH_IS_BIG_ENDIAN_)
#error "Cannot determine endianness"
#  endif

enum class Endian {
  Little = 0,
  Big    = 1,
  Native =
#  if defined(KTA_ARCH_IS_LITTLE_ENDIAN_)
  Little
#  elif defined(KTA_ARCH_IS_BIG_ENDIAN_)
  Big
#  endif
};

NODISCARD_ constexpr bool is_little_endian() noexcept {
  return Endian::Native == Endian::Little;
}

NODISCARD_ constexpr bool is_big_endian() noexcept {
  return Endian::Native == Endian::Big;
}

namespace detail_ {
  template<typename T>
  concept BitsAreSwappable = IsIntegral<T> && IsUnsigned<T> && (sizeof(T) > 1);

  template<typename T>
  NODISCARD_ constexpr auto to_unsigned(T value) noexcept -> UnsignedT<T> {
    return static_cast<UnsignedT<T>>(value);
  }

  template<typename T>
  NODISCARD_ constexpr auto from_unsigned(UnsignedT<T> value) noexcept -> T {
    return static_cast<T>(value);
  }
}

NODISCARD_ constexpr auto byteswap16(uint16 value) noexcept -> uint16 {
#  if defined(KTA_HAS_BUILTIN_BSWAP64_) && !defined(KTA_ASSUME_TESTING_ENV_)
  return __builtin_bswap16(value);
#  else
  return static_cast<uint16>((value << 8) | (value >> 8));
#  endif
}

NODISCARD_ constexpr auto byteswap32(uint32 value) noexcept -> uint32 {
#  if defined(KTA_HAS_BUILTIN_BSWAP64_) && !defined(KTA_ASSUME_TESTING_ENV_)
  return __builtin_bswap32(value);
#  else
  value = ((value << 8) & 0xFF00FF00U) | ((value >> 8) & 0x00FF00FFU);
  return (value << 16) | (value >> 16);
#  endif
}

NODISCARD_ constexpr auto byteswap64(uint64 value) noexcept -> uint64 {
#  if defined(KTA_HAS_BUILTIN_BSWAP64_) && !defined(KTA_ASSUME_TESTING_ENV_)
  return __builtin_bswap64(value);
#  else
  constexpr uint64 byte_swap_mask_hi = 0xFF00FF00FF00FF00ULL; // Mask for bytes 1,3,5,7
  constexpr uint64 byte_keep_mask_lo = 0x00FF00FF00FF00FFULL; // Mask for bytes 0,2,4,6

  constexpr uint64 word_swap_mask_hi = 0xFFFF0000FFFF0000ULL; // Mask for words 1,3
  constexpr uint64 word_keep_mask_lo = 0x0000FFFF0000FFFFULL; // Mask for words 0,2

  // 1. Swap adjacent bytes (0<->1, 2<->3, 4<->5, 6<->7)
  // 2. Swap adjacent 16-bit words (01<->23, 45<->67)
  // 3. Swap 32-bit halves (0123<->4567)

  value = ((value << 8) & byte_swap_mask_hi) | ((value >> 8) & byte_keep_mask_lo);
  value = ((value << 16) & word_swap_mask_hi) | ((value >> 16) & word_keep_mask_lo);
  return (value << 32) | (value >> 32);
#  endif
}

template<typename T> requires(sizeof(T) == 1)
NODISCARD_ constexpr auto byteswap(T value) noexcept -> T {
  return value;
}

template<typename T> requires(sizeof(T) == 2)
NODISCARD_ constexpr auto byteswap(T value) noexcept -> T {
  auto unsigned_val = detail_::to_unsigned(value);
  auto swapped = byteswap16(static_cast<uint16>(unsigned_val));
  return detail_::from_unsigned<T>(static_cast<UnsignedT<T>>(swapped));
}

template<typename T> requires(sizeof(T) == 4)
NODISCARD_ constexpr auto byteswap(T value) noexcept -> T {
  auto unsigned_val = detail_::to_unsigned(value);
  auto swapped = byteswap32(static_cast<uint32>(unsigned_val));
  return detail_::from_unsigned<T>(static_cast<UnsignedT<T>>(swapped));
}

template<typename T> requires(sizeof(T) == 8)
NODISCARD_ constexpr auto byteswap(T value) noexcept -> T{
  auto unsigned_val = detail_::to_unsigned(value);
  auto swapped = byteswap64(static_cast<uint64>(unsigned_val));
  return detail_::from_unsigned<T>(static_cast<UnsignedT<T>>(swapped));
}

template<Integer T>
NODISCARD_ constexpr auto host_to_big(T value) noexcept -> T {
  return is_little_endian() ? byteswap(value) : value;
}

template<Integer T>
NODISCARD_ constexpr auto host_to_little(T value) noexcept -> T {
  return is_big_endian() ? byteswap(value) : value;
}

template<Integer T>
NODISCARD_ constexpr auto big_to_host(T value) noexcept -> T {
  return is_little_endian() ? byteswap(value) : value;
}

template<Integer T>
NODISCARD_ constexpr auto little_to_host(T value) noexcept -> T {
  return is_big_endian() ? byteswap(value) : value;
}

template<Integer T>
NODISCARD_ auto load_big_endian(const void* ptr) noexcept -> T {
  const auto* bytes = static_cast<const uint8*>(ptr);
  T result = 0;

  for(usize i = 0; i < sizeof(T); ++i) {
    result = static_cast<T>((result << 8) | bytes[i]);
  }

  return result;
}

template<Integer T>
NODISCARD_ auto load_little_endian(const void* ptr) noexcept -> T {
  const auto* bytes = static_cast<const uint8*>(ptr);
  T result = 0;

  for(usize i = 0; i < sizeof(T); ++i) {
    result = static_cast<T>(result | (static_cast<T>(bytes[i]) << (i * 8)));
  }

  return result;
}

template<Integer T>
auto store_big_endian(void* ptr, T value) noexcept -> void {
  auto* bytes = static_cast<uint8*>(ptr);

  for(usize i = 0; i < sizeof(T); ++i) {
    bytes[i] = static_cast<uint8>(value >> ((sizeof(T) - 1 - i) * 8));
  }
}

template<Integer T>
auto store_little_endian(void* ptr, T value) noexcept -> void {
  auto* bytes = static_cast<uint8*>(ptr);

  for(usize i = 0; i < sizeof(T); ++i) {
    bytes[i] = static_cast<uint8>(value >> (i * 8));
  }
}

END_NAMESPACE_KTA_
