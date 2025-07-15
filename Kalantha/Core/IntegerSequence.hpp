/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>
#include <Kalantha/Core/Types.hpp>
BEGIN_NAMESPACE_KTA_

template<typename T, T... indices_>
struct IntegerSequence {
  using ValueType = T;
  static constexpr T size() { return sizeof...(indices_); }

  constexpr IntegerSequence& operator=(const IntegerSequence&) = default;
  constexpr IntegerSequence& operator=(IntegerSequence&&) = default;

  constexpr IntegerSequence(IntegerSequence&&) = default;
  constexpr IntegerSequence(const IntegerSequence&) = default;

  constexpr IntegerSequence() = default;
  ~IntegerSequence() = default;
};

#  if __has_builtin(__make_integer_seq)

template<typename T, usize N>
using MakeIntegerSequence = __make_integer_seq<IntegerSequence, T, N>;

#  elif __has_builtin(__integer_pack)

template<typename T, usize N>
using MakeIntegerSequence = IntegerSequence<T, __integer_pack(N)...>;

#  else
#  error "Cannot create integer sequences."
#  endif

END_NAMESPACE_KTA_