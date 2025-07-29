/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>

// Note: some cod expects this to be included from here.
// I am too lazy to refactor right now.
#include <Kalantha/Arch/Generic/WhichArch.hpp>

// =========================================
// Build platform
// =========================================
#  ifdef KTA_ASSUME_TESTING_ENV_
#  if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define KTA_BUILD_PLATFORM_WIN32

#  elif defined(__APPLE__) && defined(__MACH__)
#define KTA_BUILD_PLATFORM_DARWIN

#  elif defined(__linux__)
#define KTA_BUILD_PLATFORM_LINUX

#  else
#define KTA_BUILD_PLATFORM_UNKNOWN

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

