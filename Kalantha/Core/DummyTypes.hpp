/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>
BEGIN_NAMESPACE_KTA_

#define KTA_MAKE_DUMMY_TYPE_(TYPE_NAME, VAR_NAME) \
  struct TYPE_NAME { \
    constexpr TYPE_NAME(const TYPE_NAME&) = default; \
    constexpr TYPE_NAME(TYPE_NAME&&) = default; \
    constexpr TYPE_NAME& operator=(const TYPE_NAME&) = default; \
    constexpr TYPE_NAME& operator=(TYPE_NAME&&) = default; \
    unsigned char dummy_value_ = 0; \
    constexpr TYPE_NAME() = default; \
  }; inline constexpr TYPE_NAME VAR_NAME;

KTA_MAKE_DUMMY_TYPE_(None_, none);
KTA_MAKE_DUMMY_TYPE_(Hex_, hex);
KTA_MAKE_DUMMY_TYPE_(Dec_, dec);
KTA_MAKE_DUMMY_TYPE_(Flush_, flush);
KTA_MAKE_DUMMY_TYPE_(Endl_, endl);

END_NAMESPACE_KTA_
