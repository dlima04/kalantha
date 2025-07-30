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
BEGIN_NAMESPACE_KTA_

template<typename T>
class KtaIterator {
public:
  using PointerType   = T*;
  using ReferenceType = T&;
  using ValueType     = T;

  constexpr auto operator*(this auto&& self) -> auto&& {
    return *kta::forward<decltype(self)>(self).ptr_;
  }

  constexpr auto operator->(this auto&& self) -> auto* {
    return kta::forward<decltype(self)>(self).ptr_;
  }

  constexpr auto operator++() -> KtaIterator& {
    ++ptr_;        /// Simply increment the internal pointer
    return *this;  /// and return self.
  }

  constexpr auto operator++(int) -> KtaIterator {
    KtaIterator temp = *this;
    ++(*this);     /// Modify self, return the previous
    return temp;   /// incremented value.
  }

  constexpr auto operator--() -> KtaIterator& {
    --ptr_;        /// Simply decrement the internal pointer
    return *this;  /// and return self.
  }

  constexpr auto operator--(int) -> KtaIterator {
    KtaIterator temp = *this;
    --(*this);     /// Modify self, return the previous
    return temp;   /// incremented value.
  }

  constexpr KtaIterator& operator+=(ptrdiff n) {
    ptr_ += n;     /// increment ptr_ by n.
    return *this;  /// and return self.
  }

  constexpr KtaIterator& operator-=(ptrdiff n) {
    ptr_ -= n;     /// decrement ptr_ by n.
    return *this;  /// and return self.
  }

  constexpr KtaIterator operator+(ptrdiff n) const {
    return KtaIterator(ptr_ + n);
  }

  constexpr KtaIterator operator-(ptrdiff n) const {
    return KtaIterator(ptr_ - n);
  }

  constexpr ptrdiff operator-(const KtaIterator& other) const {
    return ptr_ - other.ptr_;
  }

  constexpr auto operator<=>(const KtaIterator& other) const = default;

  constexpr KtaIterator(PointerType ptr) : ptr_(ptr) {}
  constexpr KtaIterator(KtaIterator&&) = default;
  constexpr KtaIterator(const KtaIterator&) = default;
  constexpr KtaIterator() = default;
protected:
  PointerType ptr_{};
};

// TODO: generic reverse iterators for contiguous buffers

END_NAMESPACE_KTA_
