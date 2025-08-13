/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>
#include <Kalantha/Core/Platform.hpp>
#include <Kalantha/Core/Types.hpp>
#include <Kalantha/Core/Memory.hpp>
#include <Kalantha/Core/Assertions.hpp>
#include <Kalantha/Meta/Concepts.hpp>
#include <Kalantha/Core/Iterator.hpp>
#include <Kalantha/Core/Utility.hpp>
BEGIN_NAMESPACE_KTA_

template<Concrete T>
class Span {
public:
  using Iterator      = KtaIterator<T>;
  using ConstIterator = KtaIterator<AddConst<T>>;
  using ValueType     = RemoveCV<T>;

  constexpr Span& operator=(const Span&) = default;
  constexpr Span& operator=(Span&&)      = default;

  NODISCARD_ constexpr auto* data(this auto&& self) {
    return kta::forward<decltype(self)>(self).beg_;
  }

  NODISCARD_ constexpr Iterator begin() { return beg_; }
  NODISCARD_ constexpr Iterator end()   { return end_; }

  NODISCARD_ constexpr auto size() const -> usize {
    const ptrdiff diff = end_ - beg_; /// __PTRDIFF_TYPE__
    return static_cast<usize>(diff);
  }

  NODISCARD_ constexpr auto size_bytes() const -> usize {
    const ptrdiff diff = end_ - beg_; /// __PTRDIFF_TYPE__
    return static_cast<usize>(diff) * sizeof(T);
  }

  NODISCARD_ constexpr bool empty()          const { return !size(); }
  NODISCARD_ constexpr ConstIterator begin() const { return beg_;    }
  NODISCARD_ constexpr ConstIterator end()   const { return end_;    }

  NODISCARD_ constexpr auto&& at(this auto&& self, const usize i) {
    KTA_ASSERT(i < self.size(), "Bounds check failure");
    return (kta::forward<decltype(self)>(self).beg_)[i];
  }

  NODISCARD_ constexpr auto&& operator[](this auto&& self, const usize i) {
    return kta::forward<decltype(self)>(self).at(i);
  }

  NODISCARD_ constexpr auto&& front(this auto&& self) {
    auto&& s = kta::forward<decltype(self)>(self);
    KTA_ASSERT(s.beg_ && s.end_ && s.beg_ < s.end_, "Empty span");
    return *s.beg_;
  }

  NODISCARD_ constexpr auto&& back(this auto&& self) {
    auto&& s = kta::forward<decltype(self)>(self);
    KTA_ASSERT(s.beg_ && s.end_ && s.beg_ < s.end_, "Empty span");
    return *(s.end_ - 1);
  }

  NODISCARD_ constexpr Span subspan(usize offset, usize count) const {
    KTA_ASSERT(offset <= size(), "Subspan offset out of bounds");
    KTA_ASSERT(offset + count <= size(), "Subspan extends beyond end");
    return Span(beg_ + offset, count);
  }

  NODISCARD_ constexpr Span subspan(usize offset) const {
    KTA_ASSERT(offset <= size(), "Subspan offset out of bounds");
    return Span(beg_ + offset, size() - offset);
  }

  NODISCARD_ constexpr Span first(usize n) const {
    KTA_ASSERT(n <= size(), "First count exceeds span size");
    return Span(beg_, n);
  }

  NODISCARD_ constexpr Span last(usize n) const {
    KTA_ASSERT(n <= size(), "Last count exceeds span size");
    return Span(end_ - n, n);
  }

  NODISCARD_ constexpr Span drop_front(usize n) const {
    KTA_ASSERT(n <= size(), "Drop front count exceeds span size");
    return Span(beg_ + n, size() - n);
  }

  NODISCARD_ constexpr Span drop_back(usize n) const {
    KTA_ASSERT(n <= size(), "Drop back count exceeds span size");
    return Span(beg_, size() - n);
  }

  template<usize sz_>
  constexpr Span(T (&arr)[sz_]) : beg_(&arr[0]), end_(beg_ + sz_) {}
  constexpr Span(T* ptr, usize len) : beg_(ptr), end_(beg_ + len) {}

  constexpr Span()                  = default;
  constexpr Span(const Span& other) = default;
  constexpr Span(Span&& other)      = default;
private:
  T* beg_ = nullptr;
  T* end_ = nullptr;
};

template<typename T>
using ReadOnlySpan = Span<const T>;

END_NAMESPACE_KTA_
