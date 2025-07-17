/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>
#include <Kalantha/Core/Types.hpp>
#include <Kalantha/Core/Platform.hpp>
#include <Kalantha/Meta/Concepts.hpp>
BEGIN_NAMESPACE_KTA_

template<TotallyOrdered T>
NODISCARD_ constexpr auto min(const T& a, const T& b) -> const T& {
  return (b < a) ? b : a;
}

template<TotallyOrdered T>
NODISCARD_ constexpr auto max(const T& a, const T& b) -> const T& {
  return (a < b) ? b : a;
}

template<TotallyOrdered T>
NODISCARD_ constexpr auto clamp(const T& v, const T& lo, const T& hi) -> const T&{
  return (v < lo) ? lo : ((hi < v) ? hi : v);
}

END_NAMESPACE_KTA_
