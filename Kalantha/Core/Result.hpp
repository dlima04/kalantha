/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>
#include <Kalantha/Core/Platform.hpp>
#include <Kalantha/Core/None.hpp>
#include <Kalantha/Core/ClassTraits.hpp>
#include <Kalantha/Core/Types.hpp>
#include <Kalantha/Meta/Concepts.hpp>
#include <Kalantha/Core/Option.hpp>
BEGIN_NAMESPACE_KTA_

template<typename T, typename E>
class Result_ {
public:
  using ValueType = T;
  using ErrorType = E;

  template<typename ...Args>
  NODISCARD_ static auto create(Args&&... args) -> Result_ {
    return T{kta::forward<Args>(args)...};
  }

  template<typename ...Args>
  FORCEINLINE_ auto value_or(this auto&& self, Args&&... args) -> T {
    if(self.has_value()) return self.value();
    return T{kta::forward<Args>(args)...};
  }

  template<typename ...Args>
  FORCEINLINE_ auto error_or(this auto&& self, Args&&... args) -> E {
    if(!self.has_value()) return self.error();
    return E{kta::forward<Args>(args)...};
  }

  NODISCARD_ FORCEINLINE_ T& value()             { return *value_; }
  NODISCARD_ FORCEINLINE_ const T& value() const { return *value_; }

  NODISCARD_ FORCEINLINE_ E& error()             { return *error_; }
  NODISCARD_ FORCEINLINE_ const E& error() const { return *error_; }

  FORCEINLINE_ auto release_value(this auto&& self) -> ValueType {
    return self.value_.release_value();
  }

  FORCEINLINE_ auto release_error(this auto&& self) -> ErrorType {
    return self.error_.release_value();
  }

  template<typename C>
  auto call_if_error(this auto&& self, C&& cb) -> decltype(self) {
    if(!self.has_value()) cb( kta::forward<decltype(self)>(self) );
    return self;
  }

  template<typename C>
  auto call_if_value(this auto&& self, C&& cb) -> decltype(self) {
    if(self.has_value()) cb( kta::forward<decltype(self)>(self) );
    return self;
  }

  auto operator->(this auto&& self) -> decltype(&self.value_.value()) {
    using __PointerType = decltype(&self.value_.value());
    static_assert(IsPointer<__PointerType>, "&value_.value() does not produce a pointer!");
    return &self.value_.value();
  }

  auto operator*(this auto&& self) -> decltype(self.value_.value()) {
    using __ReferenceType = decltype(self.value_.value());
    static_assert(IsReference<__ReferenceType>, "value_.value() does not produce a reference!");
    return self.value_.value();
  }

  auto has_value() const -> bool { return value_.has_value(); }
  explicit operator bool() const { return value_.has_value(); }

  template<typename ...Args> requires kta::IsConstructible<T, Args...>
  Result_(Args&&... args) : value_(T{std::forward<Args>(args)...}){}

  Result_(Result_&& other)      = default;
  Result_(Result_ const& other) = default;
  ~Result_() = default;

  Result_(const ValueType& val) : value_(val) {}
  Result_(ValueType&& val)      : value_(kta::move(val)) {}
  Result_(const ErrorType& err) : error_(err) {}
  Result_(ErrorType&& err)      : error_(kta::move(err)) {}
  Result_(/* ...... */)         : error_(E{}) {}
private:
  Option<T> value_;
  Option<E> error_;
};

namespace detail_ {
  template<typename T>
  struct ResDispatch_ {
    using Type = T;
  };

  template<>
  struct ResDispatch_<void> {
    using Type = kta::None_;
  };

  template<typename T>
  struct ResDispatch_<T&> {
    using Type = kta::ReferenceWrapper<T>;
  };

  template<typename T>
  using ResDispatch = typename ResDispatch_<T>::Type;
}

template<typename T, typename J>
using Result = Result_<detail_::ResDispatch<T>, detail_::ResDispatch<J>>;

END_NAMESPACE_KTA_
