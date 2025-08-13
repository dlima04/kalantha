/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>
#include <Kalantha/Core/Platform.hpp>
#include <Kalantha/Core/Types.hpp>
#include <Kalantha/Core/ClassTraits.hpp>
#include <Kalantha/Meta/TypeTraits.hpp>
BEGIN_NAMESPACE_KTA_

template<typename T>
NODISCARD_ constexpr auto to_underlying(T&& t) {
  return static_cast<UnderlyingType<Decay<T>>>( t );
}

template<typename T>
NODISCARD_ constexpr auto forward(RemoveReference<T>& param) -> T&& {
  return static_cast<T&&>(param);
}

template<typename T>
NODISCARD_ constexpr auto forward(RemoveReference<T>&& param) -> T&& {
  static_assert(!IsLvalueReference<T>, "Trying to forward an rvalue as an lvalue!");
  return static_cast<T&&>(param);
}

template<typename T>
NODISCARD_ constexpr auto move(T&& obj) -> RemoveReference<T>&& {
  return static_cast<RemoveReference<T>&&>(obj);
}

template <typename T>
class ReferenceWrapper {
  KTA_MAKE_DEFAULT_CONSTRUCTIBLE(ReferenceWrapper);
  KTA_MAKE_DEFAULT_ASSIGNABLE(ReferenceWrapper);
public:
  const T& get() const { return *ptr_; }
  T& get() { return *ptr_; }
  ReferenceWrapper(T& r) : ptr_(&r) {}
private:
  T* ptr_ = nullptr;
};

END_NAMESPACE_KTA_
