/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

// =========================================
// Namespace macros
// =========================================

#define BEGIN_NAMESPACE(NAME) namespace NAME {
#define END_NAMESPACE(NAME) }

#define BEGIN_NAMESPACE_KTA_ namespace kta {
#define END_NAMESPACE_KTA_ }

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
