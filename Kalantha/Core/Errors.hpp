/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>
#include <Kalantha/Core/Platform.hpp>
#include <Kalantha/Core/Types.hpp>
#include <Kalantha/Core/ClassTraits.hpp>
BEGIN_NAMESPACE_KTA_

#define KTA_ERRC_LIST_ \
  X(None, "No error has occurred.") \
  X(NotFound, "The resource could not be located.") \
  X(InvalidArg, "An invalid argument was provided.") \
  X(NoMemory, "No memory left.") \
  X(NotImplemented, "This feature has not been implimented.") \
  X(Generic, "I couldn't think of what the error should be.")

struct ErrC {
  KTA_MAKE_COMPARABLE_MEMBER(ErrC, value);
  enum Value_ : uint16 {
#define X(VALUE, UNUSED) VALUE,
    KTA_ERRC_LIST_
#undef X
  };

  Value_ value = None;

  NODISCARD_ auto to_string()   -> const char*;
  NODISCARD_ auto description() -> const char*;

  constexpr ErrC(Value_ v) : value(v) {}
  constexpr ErrC() = default;
};

struct Error {
  using MessageType_ = const char*;
  using CodeType_    = ErrC;

  MessageType_ msg = "---";
  CodeType_ code{};

  constexpr Error(CodeType_ c) : code(c) {}
  constexpr Error(MessageType_ m, CodeType_ c) : msg(m), code(c) {}
  constexpr Error()  = default;
  constexpr ~Error() = default;
};

inline auto ErrC::to_string() -> const char* {
#define X(VALUE, UNUSED) case VALUE: return #VALUE;
  switch(this->value) {
    KTA_ERRC_LIST_
    default: return "???";
  }
#undef X
}

inline auto ErrC::description() -> const char* {
#define X(VALUE, DESC) case VALUE: return DESC;
  switch(this->value) {
    KTA_ERRC_LIST_
    default: return "???";
  }
#undef X
}

END_NAMESPACE_KTA_
