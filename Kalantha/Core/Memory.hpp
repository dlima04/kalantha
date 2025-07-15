/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>
#include <Kalantha/Core/Types.hpp>
#include <Kalantha/Core/Platform.hpp>
#include <Kalantha/Meta/Concepts.hpp>
#include <Kalantha/Core/Utility.hpp>

#  ifdef KTA_ASSUME_TESTING_ENV_
#  ifndef KTA_HAS_PLACEMENT_NEW
#define KTA_HAS_PLACEMENT_NEW
#  endif
#  endif

#  if KTA_CLANG

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

#  elif KTA_GCC

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

#  else
#error "what the fuck?"
#  endif

#ifndef KTA_HAS_PLACEMENT_NEW
#define KTA_HAS_PLACEMENT_NEW

inline void* operator new(usize size, void* ptr) noexcept {
  return ptr;
}

inline void* operator new[](usize size, void* ptr) noexcept {
  return ptr;
}

#endif //KTA_HAS_PLACEMENT_NEW
BEGIN_NAMESPACE_KTA_

template<typename T>
NODISCARD_ constexpr auto launder(T* ptr) -> T* {
  return __builtin_launder(ptr);
}

template<typename T, typename ...Args>
constexpr auto construct_at(void* ptr, Args&&... args) -> T* {
  static_assert(!IsArray<T>, "Cannot construct array type");
  return ::new (ptr) T(kta::forward<Args>(args)...);
}

template<typename T>
constexpr auto destroy_at(T* ptr) -> void {
  static_assert(!IsArray<T>, "Cannot destroy array type");
  ptr->~T();         /// TODO: array destruction should be possible
}

template<typename T, usize sz> requires(sizeof(T) > 0)
constexpr auto length_of(T (&arr)[sz]) -> usize {
  return sizeof(arr) / sizeof(T);
}

#  if KTA_CLANG
#pragma clang diagnostic pop

#  elif KTA_GCC
#pragma GCC diagnostic pop

#  endif

namespace detail_ {
  template<Character Char>
  constexpr auto strlen_(const Char* str) -> usize {
    usize len = 0;
    while(str[len] != static_cast<Char>(0)) ++len;
    return len;
  }

  template<Character Char>
  constexpr auto streq_(const Char* s1, const Char* s2) -> bool {
    const auto len1 = kta::detail_::strlen_<Char>(s1);
    const auto len2 = kta::detail_::strlen_<Char>(s2);

    if(len1 != len2) return false;
    for(usize i = 0; i < len1; i++) if(s1[ i ] != s2[ i ]) return false;
    return true;
  }
}

NODISCARD_ inline auto align_up(const usize align, void* ptr) -> void* {
  if (align == 0 || (align & (align - 1)) != 0)
    return nullptr;

  const uintptr intptr  = reinterpret_cast<uintptr>(ptr);
  const uintptr mask    = align - 1u;
  const uintptr aligned = (intptr + mask) & ~mask;
  return reinterpret_cast<void*>(aligned);
}

NODISCARD_ inline auto align_down(const usize align, void* ptr) -> void* {
  if (align == 0 || (align & (align - 1)) != 0)
    return nullptr;

  const uintptr intptr  = reinterpret_cast<uintptr>(ptr);
  const uintptr mask    = align - 1u;
  const uintptr aligned = intptr & ~mask;
  return reinterpret_cast<void*>(aligned);
}

NODISCARD_ inline auto checked_align_up(usize align, usize size, void*& ptr, usize& space) -> void* {
  if (space < size || (align & (align - 1)) != 0)
    return nullptr;       /// Check size, alignment must be a power of 2.

  const uintptr intptr  = reinterpret_cast<uintptr>(ptr);
  const uintptr aligned = (intptr + (align - 1u)) & ~(align - 1u);
  const uintptr diff    = aligned - intptr;

  if (diff + size > space) /// ensure we can update by the difference.
    return nullptr;        ///

  ptr = reinterpret_cast<void*>(aligned + size);
  space -= diff + size;
  return reinterpret_cast<void*>(aligned);
}

NODISCARD_ inline auto difference(void* start, void* end) -> usize {
  const uintptr start_ = reinterpret_cast<uintptr>(start);
  const uintptr end_   = reinterpret_cast<uintptr>(end);
  return end_ - start_;
}

END_NAMESPACE_KTA_
