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
BEGIN_NAMESPACE_KTA_

template<Character Char>
class StringView_ {
public:
  using Iterator = KtaIterator<AddConst<Char>>;
  using CharType = AddConst<RemoveVolatile<Char>>;

  NODISCARD_ constexpr CharType* data() const { return beg_;    }
  NODISCARD_ constexpr bool empty()     const { return !size(); }

  NODISCARD_ constexpr Iterator begin() const { return beg_;    }
  NODISCARD_ constexpr Iterator end()   const { return end_;    }

  NODISCARD_ constexpr auto size() const -> usize {
    const ptrdiff diff = end_ - beg_; /// produces __PTRDIFF_TYPE__
    return static_cast<usize>(diff);
  }

  NODISCARD_ constexpr auto size_bytes() const -> usize {
    const ptrdiff diff = end_ - beg_; /// produces __PTRDIFF_TYPE__
    return static_cast<usize>(diff) * sizeof(Char);
  }

  constexpr auto operator==(const StringView_& other) const -> bool {
    if(other.size() != size()) return false;
    for(usize i = 0; i < size(); ++i) if(other[i] != (*this)[i]) return false;
    return true;
  }

  constexpr auto operator!=(const StringView_& other) const -> bool {
    if(other.size() != size()) return true;
    for(usize i = 0; i < size(); ++i) if(other[i] != (*this)[i]) return true;
    return false;
  }

  NODISCARD_ constexpr CharType& at(usize i) const {
    const Char* the_ptr = beg_ + i;
    KTA_ASSERT(the_ptr < end_, "Bounds check failure");
    return *the_ptr;
  }

  NODISCARD_ constexpr StringView_ subview(usize offset, usize count) const {
    KTA_ASSERT(offset <= size(), "Subspan offset out of bounds");
    KTA_ASSERT(offset + count <= size(), "Subspan extends beyond end");
    return StringView_(beg_ + offset, count);
  }

  NODISCARD_ constexpr StringView_ subview(usize offset) const {
    KTA_ASSERT(offset <= size(), "Subspan offset out of bounds");
    return StringView_(beg_ + offset, size() - offset);
  }

  FORCEINLINE_ constexpr StringView_(CharType* ptr) {
    const usize len = detail_::strlen_(ptr);
    this->beg_ = ptr;
    this->end_ = beg_ + len;
  }

  FORCEINLINE_ constexpr StringView_(CharType* ptr, usize len) {
    this->beg_ = ptr;
    this->end_ = beg_ + len;
  }

  template<usize sz_>
  FORCEINLINE_ constexpr StringView_(const Char (&arr)[sz_]) {
    this->beg_ = &arr[0];
    this->end_ = (beg_ + sz_) - 1; /// account for null terminator.
  }

  NODISCARD_ constexpr CharType& operator[](usize i) const {
    return beg_[i];
  }

  NODISCARD_ constexpr Iterator begin() { return beg_; }
  NODISCARD_ constexpr Iterator end()   { return end_; }

  constexpr StringView_(StringView_&&)      = default;
  constexpr StringView_(StringView_ const&) = default;
  constexpr StringView_()                   = default;
private:
  AddConst<Char>* beg_ = nullptr;
  AddConst<Char>* end_ = nullptr;
};

using StringView   = StringView_<char>;
using U8StringView = StringView_<char8_t>;
using WStringView  = StringView_<wchar_t>;

END_NAMESPACE_KTA_
BEGIN_NAMESPACE(kta::string_literals);

inline auto operator ""_sv(const char* ptr, usize len) -> kta::StringView {
  return kta::StringView{ ptr, len };
}

inline auto operator ""_sv(const char8_t* ptr, usize len) -> kta::U8StringView {
  return kta::U8StringView{ ptr, len };
}

inline auto operator ""_sv(const wchar_t* ptr, usize len) -> kta::WStringView {
  return kta::WStringView{ ptr, len };
}

END_NAMESPACE(kta::string_literals);
