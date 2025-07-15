/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Assertions.hpp>
#include <Kalantha/Meta/TypeTraits.hpp>

#define TRY(EXPR)                                                                  \
  ({                                                                               \
    auto&& temp_result_ = (EXPR);                                                  \
    static_assert(!::kta::IsLvalueReference<decltype((EXPR).release_value())>);    \
    if(!temp_result_.has_value()) return temp_result_.release_error();             \
    temp_result_.release_value();                                                  \
  })                                                                               \


#define MUST(EXPR)                                                                 \
  ({                                                                               \
    auto&& temp_result_ = (EXPR);                                                  \
    static_assert(!::kta::IsLvalueReference<decltype((EXPR).release_value())>);    \
    ASSERT(temp_result_.has_value(), "Expr \"" #EXPR "\" evaluated to an error!"); \
    temp_result_.release_value();                                                  \
  })                                                                               \

