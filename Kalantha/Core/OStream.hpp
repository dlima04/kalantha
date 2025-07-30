/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>
#include <Kalantha/Core/Types.hpp>
#include <Kalantha/Meta/Concepts.hpp>
#include <Kalantha/Core/ClassTraits.hpp>
#include <Kalantha/Core/Assertions.hpp>
#include <Kalantha/Core/CharConv.hpp>

#define KTA_OSTREAM_BUFSIZE_ 1024
BEGIN_NAMESPACE_KTA_

template<usize buf_size_ = KTA_OSTREAM_BUFSIZE_>
class OStream {
  KTA_MAKE_DEFAULT_ASSIGNABLE(OStream);
  KTA_MAKE_DEFAULT_CONSTRUCTIBLE(OStream);
public:
  using OutputFn = void(*)(StringView);

  constexpr static usize len_   = buf_size_;
  constexpr static usize begin_ = 0;
  constexpr static usize end_   = len_ - 1;
  OutputFn handler = nullptr;

  FORCEINLINE_ auto to_buffer(const StringView& buff) -> OStream& {
    KTA_ASSERT(curr_ <= len_, "Invalid current buffer index.");
    KTA_ASSERT(buff.size_bytes() <= (len_ - curr_), "Buffer overrun!");
    KTA_ASSERT(buff.size_bytes() != 0, "to_buffer: empty span!");

    __builtin_memcpy(&buff_[curr_], buff.data(), buff.size_bytes());
    curr_ += buff.size_bytes();
    return *this;
  }

  FORCEINLINE_ auto flush() -> OStream& {
    KTA_ASSERT(curr_ <= len_, "Buffer overrun!");
    if(curr_ > begin_) {
      if(this->handler != nullptr) {
        this->handler(StringView(&buff_[begin_], curr_));
      }
      curr_ = begin_;       /// Reset the buffer index.
    }                       ///

    return *this;           /// return instance
  }

  auto write(const StringView& buff) -> OStream& {
    const usize remaining = len_ - curr_;
    const usize size_new  = buff.size_bytes();
    const bool  no_space  = remaining < size_new;

    if(buff.empty()) {      /// Disallow empty buffers
      return *this;
    } if(size_new > len_) { /// larger than the maximum size.
      flush();
      if(this->handler != nullptr) this->handler(buff);
    } else if(no_space) {   /// No space left. Flush the buffer.
      flush();              ///
      to_buffer(buff);
    } else {                /// There is space in the buffer. Copy
      to_buffer(buff);      /// the contents into it.
    }

    return *this;
  }

  template<Integer Int> requires(IsSigned<Int>)
  FORCEINLINE_ auto operator<<(const Int num) -> OStream& {
    char buff[40]{};

    Span<char> span(buff);
    if(kta::to_chars(num, span, base_).has_value()) {
      buff[39] = '\0'; /// Ensure null termination
      write(StringView(static_cast<char*>(buff))); /// Send buffer
    }

    return *this;
  }

  template<Integer Int> requires(IsUnsigned<Int>)
  FORCEINLINE_ auto operator<<(const Int num) -> OStream& {
    char buff[40]{};

    Span<char> span(buff);
    if(kta::to_chars(num, span, base_).has_value()) {
      buff[39] = '\0'; /// Ensure null termination
      write(StringView(static_cast<char*>(buff))); /// Send buffer
    }

    return *this;
  }

  template<Pointer Ptr> requires IsVoidPtr<Ptr>
  FORCEINLINE_ auto operator<<(const Ptr ptr) -> OStream& {
    const uint64 max_t = reinterpret_cast<uintptr>(ptr);
    char buff[40]{};

    Span<char> span(buff);
    if(kta::to_chars(max_t, span, 16).has_value()) {
      buff[39] = '\0';  /// Ensure null termination
      write(StringView(static_cast<char*>(buff))); /// Send buffer
    }

    return *this;
  }

  FORCEINLINE_ auto operator<<(const StringView& sv) -> OStream& {
    return this->write(sv);
  }

  FORCEINLINE_ auto operator<<([[maybe_unused]] const Flush_&) -> OStream& {
    return this->flush();
  }

  FORCEINLINE_ auto operator<<(const char ch) -> OStream& {
    char dummy_buf[2]{};
    dummy_buf[0] = ch;
    dummy_buf[1] = '\0';
    return this->write(StringView(static_cast<char*>(dummy_buf)));
  }

  FORCEINLINE_ auto operator<<(UNUSED_ const Endl_&) -> OStream& {
    return *this << '\n' << kta::flush;
  }

  FORCEINLINE_ auto operator<<(UNUSED_ const None_&) -> OStream& {
    return *this;
  }

  FORCEINLINE_ auto operator<<(UNUSED_ const Hex_&) -> OStream& {
    base_ = 16;
    return *this;
  }

  FORCEINLINE_ auto operator<<(UNUSED_ const Dec_&) -> OStream& {
    base_ = 10;
    return *this;
  }

  NODISCARD_ auto buffer_data()      const -> const char* { return buff_; }
  NODISCARD_ auto buffer_max_size()  const -> usize { return len_; }
  NODISCARD_ auto buffer_remaining() const -> usize { return len_ - curr_; }
  NODISCARD_ auto buffer_current()   const -> usize { return curr_; }

  constexpr OStream() = default;
  constexpr OStream(OutputFn of) : handler(of) {}
  ~OStream() = default;
private:
  char buff_[buf_size_]{};
  int base_ = 10;
  usize curr_ = begin_;
};

inline constinit OStream<> outs;
inline constinit OStream<> errs;
inline constinit OStream<> nulls;

END_NAMESPACE_KTA_
