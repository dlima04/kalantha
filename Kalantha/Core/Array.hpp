/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>
#include <Kalantha/Core/Types.hpp>
#include <Kalantha/Core/Platform.hpp>
#include <Kalantha/Core/Utility.hpp>
#include <Kalantha/Meta/Concepts.hpp>
#include <Kalantha/Core/Iterator.hpp>
#include <Kalantha/Core/Assertions.hpp>
BEGIN_NAMESPACE_KTA_

template<DefaultConstructible T, usize sz_>
class Array {
public:
  using ValueType     = RemoveCV<T>;
  using ConstIterator = KtaIterator<AddConst<T>>;
  using Iterator      = KtaIterator<T>;

  NODISCARD_ constexpr Iterator end()   { return buff_ + sz_; }
  NODISCARD_ constexpr Iterator begin() { return buff_;       }

  NODISCARD_ constexpr auto* data(this auto&& self) {
    return kta::forward<decltype(self)>(self).buff_;
  }

  constexpr auto fill(const ValueType& v) -> void {
    for(usize i = 0; i < sz_; i++) buff_[i] = v;
  }

  NODISCARD_ auto&& front(this auto&& self) {
    KTA_ASSERT(!self.empty());
    return kta::forward<decltype(self)>(self).buff_[ 0 ];
  }

  NODISCARD_ auto&& back(this auto&& self) {
    KTA_ASSERT(!self.empty());
    return kta::forward<decltype(self)>(self).buff_[ sz_ - 1 ];
  }

  NODISCARD_ auto&& at(this auto&& self, usize i) {
    KTA_ASSERT(i < self.size(), "Bounds check failure!");
    return kta::forward<decltype(self)>(self).buff_[ i ];
  }

  NODISCARD_ constexpr auto&& operator[](this auto&& self, usize i) {
    return kta::forward<decltype(self)>(self).buff_[ i ];
  }

  NODISCARD_ constexpr ConstIterator end()   const { return buff_ + sz_; }
  NODISCARD_ constexpr ConstIterator begin() const { return buff_;       }

  constexpr Array(T (&arr)[sz_]) {
    for(usize i = 0; i < sz_; i++) buff_[ i ] = arr[ i ];
  }

  constexpr Array(T (&&arr)[sz_]) {
    for(usize i = 0; i < sz_; i++) buff_[ i ] = kta::move(arr[ i ]);
  }

  template<AreAll<T> ...Args>
  FORCEINLINE_ constexpr Array(Args&&... args) : buff_(kta::forward<Args>(args)...)  {}

  NODISCARD_ constexpr usize size() const { return sz_;      }
  NODISCARD_ constexpr bool empty() const { return sz_ == 0; }

  constexpr Array() = default;
  constexpr Array(const Array&) = default;
  constexpr Array(Array&&) = default;
private:
  T buff_[ sz_ ]{};
};

END_NAMESPACE_KTA_

/// For structured bindings.
namespace std {
  template<typename T, usize sz_>
  struct tuple_size<::kta::Array<T, sz_>> {
    static constexpr usize value = sz_;
  };

  template<usize I, typename T, usize sz_>
  struct tuple_element<I, kta::Array<T, sz_>> {
    static_assert(I < sz_, "Index out of bounds in tuple_element for Array.");
    using type = T;
  };
}
