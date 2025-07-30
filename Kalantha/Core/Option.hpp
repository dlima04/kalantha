/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>
#include <Kalantha/Core/Platform.hpp>
#include <Kalantha/Core/Types.hpp>
#include <Kalantha/Core/Utility.hpp>
#include <Kalantha/Meta/Concepts.hpp>
#include <Kalantha/Core/Assertions.hpp>
#include <Kalantha/Core/Memory.hpp>
#include <Kalantha/Core/ClassTraits.hpp>
#include <Kalantha/Core/DummyTypes.hpp>

BEGIN_NAMESPACE_KTA_

template <Concrete T>
class Option_ {
public:
  using ValueType     = T;
  using ReferenceType = T&;
  using PointerType   = T*;

  NODISCARD_ FORCEINLINE_ auto value() & -> ReferenceType {
    KTA_ASSERT(has_value_, "Option_ has no contained type!");
    return *kta::launder<T>(reinterpret_cast<T*>(&value_));
  }

  NODISCARD_ FORCEINLINE_ auto value() const& -> const T& {
    KTA_ASSERT(has_value_, "Option_ has no contained type!");
    return *kta::launder<const T>(reinterpret_cast<const T*>(&value_));
  }

  NODISCARD_ FORCEINLINE_ auto value() && -> ValueType {
    KTA_ASSERT(has_value_, "Option_ has no contained type!");
    return release_value(); /// rvalues should release the contained type.
  }                         ///

  auto operator->(this auto&& self) -> decltype(auto) {
    KTA_ASSERT(self.has_value_, "Option_ has no contained type!");
    return &self.value();
  }

  auto operator*(this auto&& self) -> decltype(auto) {
    KTA_ASSERT(self.has_value_, "Option_ has no contained type!");
    return self.value();
  }

  FORCEINLINE_ auto release_value() -> ValueType {
    KTA_ASSERT(has_value_, "Option_ has no contained type!");
    T released = kta::move( value() );
    value().~T();           /// We'll have to manually call the destructor.
    has_value_ = false;     ///
    return released;
  }

  FORCEINLINE_ auto clear() -> void {
    if(has_value_)
      value().~T();
    has_value_ = false;
  }

  FORCEINLINE_ auto value_or(T&& other) const -> ValueType {
    if(has_value_) return value();
    return other;
  }

  FORCEINLINE_ auto operator=(Option_&& other) noexcept -> Option_& {
    if(&other == this) return *this;
    clear();
    if(other.has_value_) {
      has_value_ = true;
      kta::construct_at<T>(&value_, other.release_value());
    }

    return *this;
  }

  FORCEINLINE_ auto operator=(const Option_& other) -> Option_& {
    if(&other == this) return *this;
    clear();
    if(other.has_value_) {
      has_value_ = true;
      kta::construct_at<T>(&value_, other.value());
    }

    return *this;
  }

  template<typename ...Args>
  FORCEINLINE_ auto emplace(Args&&... args) -> void {
    clear();
    has_value_ = true;
    kta::construct_at<T>(&value_, kta::forward<Args>(args)...);
  }

  template<typename ...Args>
  FORCEINLINE_ static auto create(Args&&... args) -> Option_ {
    return Option_{ kta::forward<Args>(args)... };
  }

  template<typename ...Args> requires Constructs<T, Args...>
  FORCEINLINE_ Option_(Args&&... args) {
    kta::construct_at<T>(&value_, kta::forward<Args>(args)...);
    has_value_ = true;
  }

  FORCEINLINE_ Option_(const Option_& other) {
    if(!other.has_value_) return;
    kta::construct_at<T>( &value_, other.value() );
    has_value_ = true;
  }

  FORCEINLINE_ Option_(Option_&& other) {
    if(!other.has_value_) return;
    kta::construct_at<T>( &value_, other.release_value() );
    has_value_ = true;
  }

  auto has_value() const -> bool { return has_value_; }
  explicit operator bool() const { return has_value_; }

  Option_() = default;
  Option_(const None_&) {}
 ~Option_() { clear(); }
private:
  bool has_value_{false};
  alignas(T) uint8 value_[ sizeof(T) ]{};
};

namespace detail_ {
  template<typename T>
  struct OptionDispatch {
    using Type = kta::Option_<T>;
  };

  template<>
  struct OptionDispatch<void> {
    using Type = kta::Option_<None_>;
  };

  template<typename T>
  struct OptionDispatch<T&> {
    using Type = kta::Option_<kta::ReferenceWrapper<T>>;
  };
}

template<typename T>
using Option = typename detail_::OptionDispatch<T>::Type;

END_NAMESPACE_KTA_
