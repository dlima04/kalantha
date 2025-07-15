/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once
#include <Kalantha/Core/Common.hpp>
BEGIN_NAMESPACE_KTA_

struct None_ {
  unsigned char dummy_value_ = 0;
  constexpr None_() = default;
};

constexpr inline None_ None;

END_NAMESPACE_KTA_
