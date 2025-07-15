/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>

// =========================================
// Architecture
// =========================================
#  if defined(__x86_64__) || defined(_M_X64)
#define ARCH_X86_64 1
#  elif defined(__i386__) || defined(_M_IX86)
#define ARCH_X86 1
#  endif //if defined(__x86_64__) || defined(_M_X64)

#  if defined(__aarch64__) || defined(_M_ARM64)
#define ARCH_ARM64 1
#  elif defined(__arm__) || defined(_M_ARM)
#define ARCH_ARM 1
#  endif //defined(__aarch64__) || defined(_M_ARM64)

#  if defined(__mips__) || defined(__mips) || defined(__MIPS__)
#define ARCH_MIPS 1
#  if defined(__mips64)
#define ARCH_MIPS64 1
#  else
#define ARCH_MIPS32 1
#  endif //defined(__mips64)
#  endif //...

#  if defined(__powerpc64__) || defined(__ppc64__) || defined(__PPC64__)
#define ARCH_PPC64 1
#  elif defined(__powerpc__) || defined(__ppc__) || defined(__PPC__)
#define ARCH_PPC 1
#  endif //...

#  if defined(__riscv)
#define ARCH_RISCV 1
#  if __riscv_xlen == 64
#define ARCH_RISCV64 1
#  elif __riscv_xlen == 32
#define ARCH_RISCV32 1
#  endif //__riscv_xlen == 64
#  endif //defined(__riscv)

#  if defined(__sparc__) || defined(__sparc)
#define ARCH_SPARC 1
#  if defined(__arch64__)
#define ARCH_SPARC64 1
#  else
#define ARCH_SPARC32 1
#  endif
#endif //defined(__sparc__) || defined(__sparc)

#  if defined(__ia64__) || defined(_M_IA64)
#define ARCH_IA64 1
#  endif //defined(__ia64__) || defined(_M_IA64)

#  if defined(__s390x__)
#define ARCH_S390X 1
#  elif defined(__s390__)
#define ARCH_S390 1
#  endif //defined(__s390x__)

#  if defined(__alpha__)
#define ARCH_ALPHA 1
#  endif //defined(__alpha__)

#  if defined(__hppa__)
#define ARCH_PARISC 1
#  endif //defined(__hppa__)

#  if defined(__sh__)
#define ARCH_SH 1
#  endif //defined(__sh__)

// =========================================
// Build platform (only for tests)
// =========================================
#  ifdef KTA_ASSUME_TESTING_ENV_
#  if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define KTA_BUILD_PLATFORM_WIN32

#  elif defined(__APPLE__) && defined(__MACH__)
#define KTA_BUILD_PLATFORM_DARWIN

#  elif defined(__linux__)
#define KTA_BUILD_PLATFORM_LINUX

#  else
#error "Unknown build platform"
#  endif

#  if defined(KTA_BUILD_PLATFORM_DARWIN) || defined(KTA_BUILD_PLATFORM_LINUX)
#define KTA_BUILD_PLATFORM_POSIX
#  endif

#  endif //KTA_ASSUME_TESTING_ENV_

// =========================================
// Compiler detection
// =========================================
#   if defined(__clang__)
#define KTA_GCC   0
#define KTA_CLANG 1
#   elif defined(__GNUC__)
#define KTA_GCC   1
#define KTA_CLANG 0
#   else
#define KTA_GCC   0
#define KTA_CLANG 0
#error "Core/Platform.hpp: Unsupported compiler!"
#   endif //defined(__clang__)

// =========================================
// Cpp attributes
// =========================================
#define NODISCARD_    [[nodiscard]]
#define UNUSED_       [[maybe_unused]]
#define NORETURN_     [[noreturn]]
#define FALLTHROUGH_  [[fallthrough]]

// =========================================
// GNU/Clang attributes
// =========================================
#define NAKED_        __attribute__((naked))
#define PACKED_       __attribute__((packed))
#define FORCEINLINE_  __attribute__((always_inline)) inline
#define NOINLINE_     __attribute__((noinline))
#define WEAK_         __attribute__((weak))
