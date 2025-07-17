/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>
#include <Kalantha/Meta/TypeTraits.hpp>
BEGIN_NAMESPACE_KTA_

template<typename T, typename ...Us>
concept AreAll = (IsSame<T, Decay<Us>> && ...);

template<typename T, typename ...Us>
concept AnyOf = (IsSame<T, Decay<Us>> || ...);

template<typename T, typename U>
concept Is = IsSame<T, U>;

template<typename T>
concept Concrete = requires { !IsReference<T>; };

template<typename T>
concept Integer = IsIntegral<T>;

template<typename T>
concept Signed = IsIntegral<T> && IsSigned<T>;

template<typename T>
concept Unsigned = IsIntegral<T> && IsUnsigned<T>;

template<typename T>
concept Pointer = IsPointer<T>;

template<typename T>
concept Character = IsCharacter<T>;

template<typename T>
concept TrivialDTOR = IsTriviallyDestructible<T>;

template<typename T>
concept TrivialCTOR = IsTriviallyConstructible<T>;

template<typename T>
concept DefaultConstructible = IsDefaultConstructible<T>;

template<typename T, typename ...Args>
concept Constructs = IsDestructible<T> && IsConstructible<T, Args...>;

template<typename From, typename To>
concept ConvertibleTo = IsConvertible<From, To>;

template<typename T>
concept Comparable = requires(T a, T b) {
  {a == b} -> ConvertibleTo<bool>;
  {a != b} -> ConvertibleTo<bool>;
};

template<typename T>
concept TotallyOrdered = requires(T a, T b) {
  {a == b} -> ConvertibleTo<bool>;
  {a != b} -> ConvertibleTo<bool>;
  {a <  b} -> ConvertibleTo<bool>;
  {a <= b} -> ConvertibleTo<bool>;
  {a >  b} -> ConvertibleTo<bool>;
  {a >= b} -> ConvertibleTo<bool>;
};

END_NAMESPACE_KTA_
