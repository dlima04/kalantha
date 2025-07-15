/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#define KTA_MAKE_COMPARABLE_IMPL_(OP, MB, CLASS)      \
  bool operator OP (const CLASS& other) const {       \
    return other OP this->MB;                         \
  }                                                   \

#define KTA_MAKE_COMPARABLE_MEMBER_IMPL_(C, MB, OP)   \
  bool operator OP (const C& other) const {           \
    return other.MB OP this->MB;                      \
  }                                                   \

#define KTA_MAKE_COMPARABLE_ON(CLASS, MB)             \
  public:                                             \
    KTA_MAKE_COMPARABLE_IMPL_(==, MB, CLASS)          \
    KTA_MAKE_COMPARABLE_IMPL_(!=, MB, CLASS)          \

#define KTA_MAKE_COMPARABLE_MEMBER(SELF, MB)          \
  public:                                             \
    KTA_MAKE_COMPARABLE_MEMBER_IMPL_(SELF, MB, ==)    \
    KTA_MAKE_COMPARABLE_MEMBER_IMPL_(SELF, MB, !=)    \

#define KTA_MAKE_NONCOPYABLE(C)                       \
  private:                                            \
    C(const C&) = delete;                             \
    C& operator =(const C&) = delete                  \

#define KTA_MAKE_NONMOVABLE(C)                        \
  private:                                            \
    C(C&&) = delete;                                  \
    C& operator=(C&&) = delete                        \

#define KTA_MAKE_DEFAULT_COPY_CONSTRUCTIBLE(C)        \
  public: C(const C& other) = default;                \

#define KTA_MAKE_DEFAULT_MOVE_CONSTRUCTIBLE(C)        \
  public: C(C&& other) = default;                     \

#define KTA_MAKE_DEFAULT_COPY_ASSIGNABLE(C)           \
  public: C& operator=(const C& other) = default;     \

#define KTA_MAKE_DEFAULT_MOVE_ASSIGNABLE(C)           \
  public: C& operator=(C&& other) = default;          \

#define KTA_MAKE_DEFAULT_ASSIGNABLE(C)                \
  KTA_MAKE_DEFAULT_COPY_ASSIGNABLE(C)                 \
  KTA_MAKE_DEFAULT_MOVE_ASSIGNABLE(C)                 \

#define KTA_MAKE_DEFAULT_CONSTRUCTIBLE(C)             \
  KTA_MAKE_DEFAULT_MOVE_CONSTRUCTIBLE(C)              \
  KTA_MAKE_DEFAULT_COPY_CONSTRUCTIBLE(C)              \

