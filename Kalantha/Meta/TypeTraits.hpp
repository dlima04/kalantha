/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>
BEGIN_NAMESPACE_KTA_

template<typename...>
using VoidType = void;

template<typename T, typename = void>
struct AddReference_ {
  using LvalueType = T;
  using TvalueType = T;
};

template<typename T>
struct AddReference_<T, VoidType<T&>> {
  using LvalueType = T&;
  using RvalueType = T&&;
};

template<typename T>
struct MakeUnsigned_ {
  using Type = void;
};

template<>
struct MakeUnsigned_<signed char> {
  using Type = unsigned char;
};

template<>
struct MakeUnsigned_<short> {
  using Type = unsigned short;
};

template<>
struct MakeUnsigned_<int> {
  using Type = unsigned int;
};

template<>
struct MakeUnsigned_<long> {
  using Type = unsigned long;
};

template<>
struct MakeUnsigned_<long long> {
  using Type = unsigned long long;
};

template<>
struct MakeUnsigned_<unsigned char> {
  using Type = unsigned char;
};

template<>
struct MakeUnsigned_<unsigned short> {
  using Type = unsigned short;
};

template<>
struct MakeUnsigned_<unsigned int> {
  using Type = unsigned int;
};

template<>
struct MakeUnsigned_<unsigned long> {
  using Type = unsigned long;
};

template<>
struct MakeUnsigned_<unsigned long long> {
  using Type = unsigned long long;
};

template<>
struct MakeUnsigned_<char> {
  using Type = unsigned char;
};

template<>
struct MakeUnsigned_<char8_t> {
  using Type = char8_t;
};

template<>
struct MakeUnsigned_<char16_t> {
  using Type = char16_t;
};

template<>
struct MakeUnsigned_<char32_t> {
  using Type = char32_t;
};

template<>
struct MakeUnsigned_<bool> {
  using Type = bool;
};

template<typename T>
using AddConst = T const;

template<typename T>
using AddVolatile = T volatile;

template<typename T>
using AddPtr = T*;

template<typename T>
using MakeUnsigned = typename MakeUnsigned_<T>::Type;

template<typename T>
using AddLvalueReference = typename AddReference_<T>::LvalueType;

template<typename T>
using AddRvalueReference = typename AddReference_<T>::RvalueType;

template<typename T>
typename AddReference_<T>::RvalueType declval();

template<typename T>
struct RemoveReference_ {
  using Type = T;
};

template<typename T>
struct RemoveReference_<T&> {
  using Type = T;
};

template<typename T>
struct RemoveReference_<T&&> {
  using Type = T;
};

template<typename T>
struct RemovePointer_ {
  using Type = T;
};

template<typename T>
struct RemovePointer_<T*> {
  using Type = T;
};

template<typename T>
struct RemoveConst_ {
  using Type = T;
};

template<typename T>
struct RemoveConst_<T const> {
  using Type = T;
};

template<typename T, T val_>
struct IntegralConstant {
  static constexpr T value = val_;
  using ValueType = T;

  constexpr operator ValueType()   const { return value; }
  constexpr ValueType operator()() const { return value; }
};

template<bool val_>
using BoolConstant = IntegralConstant<bool, val_>;

using TrueConstant = BoolConstant<true>;
using FalseConstant = BoolConstant<false>;

template<typename T>
struct FalseType : FalseConstant {};

template<typename T>
struct TrueType : TrueConstant {};

template<typename T>
using RemoveConst = typename RemoveConst_<T>::Type;

template<typename T>
using RemoveReference = typename RemoveReference_<T>::Type;

template<typename T>
using RemovePointer = typename RemovePointer_<T>::Type;

template<typename T>
struct RemoveVolatile_ {
  using Type = T;
};

template<typename T>
struct RemoveVolatile_<T volatile> {
  using Type = T;
};

template<typename T>
using RemoveVolatile = typename RemoveVolatile_<T>::Type;

template<typename T>
using RemoveCV = RemoveVolatile<RemoveConst<T>>;

template<typename T>
using Decay = RemoveReference<RemoveCV<T>>;

template<typename T>
inline constexpr bool IsIntegral_ = false; //base

template<> inline constexpr bool IsIntegral_<bool>                = true;
template<> inline constexpr bool IsIntegral_<unsigned char>       = true;
template<> inline constexpr bool IsIntegral_<unsigned short>      = true;
template<> inline constexpr bool IsIntegral_<unsigned int>        = true;
template<> inline constexpr bool IsIntegral_<unsigned long>       = true;
template<> inline constexpr bool IsIntegral_<unsigned long long>  = true;
template<> inline constexpr bool IsIntegral_<char8_t>             = true;
template<> inline constexpr bool IsIntegral_<char16_t>            = true;
template<> inline constexpr bool IsIntegral_<char32_t>            = true;

template<typename T>
inline constexpr bool IsIntegral = IsIntegral_<Decay<MakeUnsigned<T>>>;

template<typename T> inline constexpr bool IsLvalueReference      = false;
template<typename T> inline constexpr bool IsLvalueReference<T&>  = true;
template<typename T> inline constexpr bool IsRvalueReference      = false;
template<typename T> inline constexpr bool IsRvalueReference<T&&> = true;
template<typename T> inline constexpr bool IsReference            = false;
template<typename T> inline constexpr bool IsReference<T&>        = true;
template<typename T> inline constexpr bool IsReference<T&&>       = true;

template<typename T>
inline constexpr bool IsFloatingPoint_ = false; //base

template<> inline constexpr bool IsFloatingPoint_<double>         = true;
template<> inline constexpr bool IsFloatingPoint_<long double>    = true;
template<> inline constexpr bool IsFloatingPoint_<float>          = true;

template<typename T>
inline constexpr bool IsFloatingPoint = IsFloatingPoint_<Decay<T>>;

template<typename T>
inline constexpr bool IsCharacter_ = false; //base

template<> inline constexpr bool IsCharacter_<char8_t>            = true;
template<> inline constexpr bool IsCharacter_<char16_t>           = true;
template<> inline constexpr bool IsCharacter_<char32_t>           = true;
template<> inline constexpr bool IsCharacter_<char>               = true;
template<> inline constexpr bool IsCharacter_<wchar_t>            = true;

template<typename T>
inline constexpr bool IsCharacter = IsCharacter_<Decay<T>>;

template<typename T> inline constexpr bool IsPointer_             = false;
template<typename T> inline constexpr bool IsPointer_<T*>         = true;

template<typename T>
inline constexpr bool IsPointer = IsPointer_<RemoveCV<T>>;

template<typename T, typename ...Us>
inline constexpr bool IsInvocableWith = requires { T(declval<Us>()...); };

template<typename T>
inline constexpr bool IsVoid = false;

template<>
inline constexpr bool IsVoid<void> = true;

template<typename T>
struct Unsigned_ {
  using Type = void;
};

template<>
struct Unsigned_<signed char> {
  using Type = unsigned char;
};

template<>
struct Unsigned_<short> {
  using Type = unsigned short;
};

template<>
struct Unsigned_<int> {
  using Type = unsigned int;
};

template<>
struct Unsigned_<long> {
  using Type = unsigned long;
};

template<>
struct Unsigned_<long long> {
  using Type = unsigned long long;
};

template<>
struct Unsigned_<unsigned char> {
  using Type = unsigned char;
};

template<>
struct Unsigned_<unsigned short> {
  using Type = unsigned short;
};

template<>
struct Unsigned_<unsigned int> {
  using Type = unsigned int;
};

template<>
struct Unsigned_<unsigned long> {
  using Type = unsigned long;
};

template<>
struct Unsigned_<unsigned long long> {
  using Type = unsigned long long;
};

template<>
struct Unsigned_<char> {
  using Type = unsigned char;
};

template<>
struct Unsigned_<char8_t> {
  using Type = char8_t;
};

template<>
struct Unsigned_<char16_t> {
  using Type = char16_t;
};

template<>
struct Unsigned_<char32_t> {
  using Type = char32_t;
};

template<>
struct Unsigned_<bool> {
  using Type = bool;
};

template<typename T>
using UnsignedT = typename Unsigned_<T>::Type;

template<typename T>
struct Signed_ {
  using Type = void;
};

template<>
struct Signed_<short> {
  using Type = short;
};

template<>
struct Signed_<int> {
  using Type = int;
};

template<>
struct Signed_<long> {
  using Type = long;
};

template<>
struct Signed_<long long> {
  using Type = long long;
};

template<>
struct Signed_<unsigned char> {
  using Type = char;
};

template<>
struct Signed_<unsigned short> {
  using Type = short;
};

template<>
struct Signed_<unsigned int> {
  using Type = int;
};

template<>
struct Signed_<unsigned long> {
  using Type = long;
};

template<>
struct Signed_<unsigned long long> {
  using Type = long long;
};

template<>
struct Signed_<char> {
  using Type = char;
};

template<typename T>
using SignedT = typename Signed_<T>::Type;

template<typename T, typename U>
inline constexpr bool IsSame = false;

template<typename T>
inline constexpr bool IsSame<T, T> = true;

template<typename T>
inline constexpr bool IsConst = IsSame<T, AddConst<T>>;

template<typename T>
inline constexpr bool IsVolatile = IsSame<T, AddVolatile<T>>;

template<typename T>
inline constexpr bool IsUnsigned = IsSame<T, UnsignedT<T>>;

template<typename T>
inline constexpr bool IsSigned = IsSame<T, SignedT<T>>;

template<typename T>
inline constexpr bool IsVoidPtr = IsPointer<T> && !IsCharacter<RemovePointer<T>>;

template<typename T>
inline constexpr bool IsAbstract = __is_abstract(T);

template<typename T>
inline constexpr bool IsArray = __is_array(T);

template<typename Base, typename Derived>
inline constexpr bool IsBaseOf = __is_base_of(Base, Derived);

template<typename T, typename ...Args>
inline constexpr bool IsTriviallyConstructible = __is_trivially_constructible(T, Args...);

template<typename T>
inline constexpr bool IsDestructible = requires { declval<T>().~T(); };

#  if __has_builtin(__is_trivially_destructible)

template<typename T>
inline constexpr bool IsTriviallyDestructible = __is_trivially_destructible(T);

#  elif __has_builtin(__has_trivial_destructor)

template<typename T>
inline constexpr bool IsTriviallyDestructible = __has_trivial_destructor(T);

#  else
#error "Cannot detect trivial destructors"
#  endif

template<typename T, typename ...Args>
inline constexpr bool IsConstructible = __is_constructible(T, Args...);

template<typename T>
inline constexpr bool IsDefaultConstructible = __is_constructible(T);

template<typename T>
using UnderlyingType = __underlying_type(T);

template<typename From, typename To>
inline constexpr bool IsConvertible = __is_convertible(From, To);

END_NAMESPACE_KTA_
