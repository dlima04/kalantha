/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>
#include <Kalantha/Core/Types.hpp>
#include <Kalantha/Core/Platform.hpp>
#include <Kalantha/Core/Utility.hpp>

#  ifdef KTA_ASSUME_TESTING_ENV_
#include <iostream>
#include <stdlib.h>

#  ifdef __has_include
#  if __has_include(<cxxabi.h>)
#include <cxxabi.h>
#define KTA_HAS_CXXABI_H_
#  endif //__has_include(<cxxabi.h>)
#  endif //__has_include

#  endif //KTA_ASSUME_TESTING_ENV_

#  ifdef __PRETTY_FUNCTION__
#define KTA_FUNC_ __PRETTY_FUNCTION__
#  else
#define KTA_FUNC_ __func__
#  endif

#ifndef KTA_ASSERTIONS_OFF_
#define KTA_PANIC(MSG) ::kta::detail_::panic_impl_( \
  MSG, \
  KTA_FUNC_, \
  __FILE__, \
  __LINE__) \

#define KTA_UNREACHABLE() ::kta::detail_::panic_impl_( \
  "Unreachable branch executed", \
  KTA_FUNC_, \
  __FILE__, \
  __LINE__) \

#define KTA_ASSERT(COND, ...) if(!(COND)) { ::kta::detail_::panic_impl_( \
  "Assertion \"" #COND "\" failed! " __VA_ARGS__, \
  KTA_FUNC_, \
  __FILE__, \
  __LINE__); } \

#  else
#define KTA_PANIC(MSG)
#define KTA_UNREACHABLE()
#define KTA_ASSERT(COND, ...)
#  endif //KTA_ASSERTIONS_OFF_

#  ifdef  KTA_ASSUME_TESTING_ENV_
#  if     defined(KTA_HAS_CXXABI_H_) && defined(KTA_BUILD_PLATFORM_POSIX)
#  define KTA_BACKTRACE_MAX_FRAMES 40

#include <unistd.h>
#include <execinfo.h>
#include <dlfcn.h>

inline auto kta_display_backtrace_() -> void {
  void *trace[KTA_BACKTRACE_MAX_FRAMES]{};
  ::Dl_info dlinfo{};

  int status = 0;
  const char* symname = nullptr;
  char* demangled = nullptr;

  int trace_size = ::backtrace(trace, KTA_BACKTRACE_MAX_FRAMES);
  for(int i = 0; i < trace_size; i++) {
    if(!::dladdr(trace[i], &dlinfo))
      continue;

    symname = dlinfo.dli_sname;
    demangled = abi::__cxa_demangle(symname, nullptr, nullptr, &status);

    if(status == 0 && demangled) {
      symname = demangled;
    } else {
      return;
    }

    std::cout << "At " << trace[i] << " " << symname << "\n";

    if (demangled)
      ::free(demangled);
  }

  std::cout
    << "\nTraced "
    << trace_size
    << " frames out of "
    << KTA_BACKTRACE_MAX_FRAMES
    << " max.\n";
}

#  else //NO KTA_HAS_CXXABI_H_

inline auto kta_display_backtrace_() -> void { /*...*/ }

#  endif
#  endif //KTA_ASSUME_TESTING_ENV_

BEGIN_NAMESPACE_KTA_

struct PanicInfo {
  const char* msg;
  const char* fn_name;
  const char* file;
  int line;
};

using PanicFn = void (*)(const PanicInfo&);
inline constinit PanicFn panic_handler_ = nullptr;

namespace detail_ {
  NORETURN_ inline auto panic_impl_(
    const char* msg_,  const char* fn_name_,
    const char* file_, const int line_ ) -> void
  {
    PanicInfo info;
    info.msg = msg_;
    info.fn_name = fn_name_;
    info.file = file_;
    info.line = line_;

    if(panic_handler_ != nullptr) {
      panic_handler_(info);
    }
#ifdef KTA_ASSUME_TESTING_ENV_
    else {
      std::cout
        << "PANIC: "
        << info.msg
        << "\nIn "
        << info.file
        << ":"
        << info.line
        << ",\nIn function "
        << info.fn_name
        << "\n\n";
      ::kta_display_backtrace_();

      std::cout.flush();
      std::cerr.flush();

      ::exit(1);
    }
#endif

    __builtin_unreachable();
  }
}

END_NAMESPACE_KTA_
