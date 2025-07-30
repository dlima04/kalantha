/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Types.hpp>
#include <Kalantha/Core/Span.hpp>
#include <Kalantha/Core/Common.hpp>
#include <Kalantha/Core/Result.hpp>
#include <Kalantha/Core/Limits.hpp>
#include <Kalantha/Core/StringView.hpp>
#include <Kalantha/Core/Errors.hpp>
#include <Kalantha/Core/Option.hpp>
#include <Kalantha/Core/Try.hpp>
BEGIN_NAMESPACE_KTA_

constexpr int BaseHex = 16;
constexpr int BaseDec = 10;
constexpr int BaseOct = 8;
constexpr int BaseBin = 2;

constexpr char tolower(char c) { return c + static_cast<char>(0x20); }
constexpr char toupper(char c) { return c - static_cast<char>(0x20); }
constexpr bool isspace(char c) { return c == ' ' || c == '\t' || c == '\n'; }
constexpr bool isascii(char c) { return c >= 0; }
constexpr bool isdigit(char c) { return c >= '0' && c <= '9'; }

/* ASCII conversion functions.
* note: there is no handling for floating point values at the moment.
* this isn't a huge priority but it should be addressed eventually.
*
* the functionality here is intentionally similar to std::from/to _chars.
* we operate on fixed size character buffers for conversions.
*/

BEGIN_NAMESPACE(detail_);

template<Integer Int>
auto digit_value_(const char ch) -> Option<Int> {
  if (ch >= '0' && ch <= '9') return ch - '0';
  if (ch >= 'A' && ch <= 'F') return ch - 'A' + 10;
  if (ch >= 'a' && ch <= 'f') return ch - 'a' + 10;
  return kta::none;
}

inline auto is_digit_(char ch, int base) -> bool {
  if(ch >= 'A' && ch <= 'Z') ch = tolower(ch);
  switch (base) {
    case BaseBin: return ch == '1' || ch == '0';
    case BaseHex: return (ch >= 'a' && ch <= 'f') || (ch >= '0' && ch <= '9');
    case BaseDec: return ch >= '0' && ch <= '9';
    case BaseOct: return ch >= '0' && ch <= '7';
    default: break;
  }

  return false;
}

template<Integer Int, Int base_>
auto from_chars_(const StringView& sv, Int& out) -> Result<void, Error> {
  out = 0; bool neg = false;
  if(sv.empty()) return Error{ErrC::InvalidArg};
  if(sv == "0")  return Result<void, Error>::create();

  usize index;
  for(index = 0; index < sv.size() && isspace(sv[index]); ++index);
  if(index >= sv.size()) return Error{ErrC::InvalidArg};

  if(const char ch = sv.at(index); ch == '+' || ch == '-') {
    neg = ch == '-'; /// Two's complement negation still required,
    ++index;         /// even for unsigned types.
  }

  for(; index < sv.size() && detail_::is_digit_(sv.at(index), base_); ++index) {
    const Int digit = MUST(detail_::digit_value_<Int>(sv.at(index)));
    const Int maxi  = NumericLimits<Int>::max();
    if(out > (maxi - digit) / base_) return Error{ErrC::Overflow};
    out = out * base_ + digit;
  }

  if(neg == true) {
    const bool sign = NumericLimits<Int>::is_signed;
    const Int mini  = NumericLimits<Int>::min();
    if (out == mini && sign) return Error{ErrC::Overflow};
    out = -out;
  }

  return Result<void, Error>::create();
}

inline auto append_next_char_(Span<char>& chars, usize& i, char ch) -> Result<void, Error> {
  if(i >= chars.size()) return Error{"buffer too small!", ErrC::Overflow};
  chars.at(i++) = ch;
  return Result<void, Error>::create();
}

template<Integer Int, Int base_>
auto to_chars_(const Int num, Span<char>& chars) -> Result<usize, Error> {
  if(usize temp_index = 0; num == 0) {
    TRY(append_next_char_(chars, temp_index, '0'));
    return Result<usize, Error>::create(1ULL);
  }

  usize index = 0, curr = num;
  if (NumericLimits<Int>::is_signed && num < 0) {
    curr = -curr;
  }

  while(curr > 0) {
    usize rem  = curr % base_;
    char digit = rem < 10 ? rem + '0' : (rem - 10) + 'A';
    TRY(append_next_char_(chars, index, digit));
    curr /= base_;
  }

  if(NumericLimits<Int>::is_signed && num < 0) {
    TRY(append_next_char_(chars, index, '-'));
  }

  switch(base_) {
    case 16:
      TRY(append_next_char_(chars, index, 'x'));
      TRY(append_next_char_(chars, index, '0'));
      break;
    case 8:
      TRY(append_next_char_(chars, index, '0'));
      break;
    case 2:
      TRY(append_next_char_(chars, index, 'b'));
      TRY(append_next_char_(chars, index, '0'));
      FALLTHROUGH_;
    default: break;
  }

  for(usize start = 0, end = index - 1; start < end; ++start, --end) {
    const char temp = chars.at(start); /// Reverse the characters.
    chars.at(start) = chars.at(end);   ///
    chars.at(end)   = temp;
  }

  return Result<usize, Error>::create(index);
}

END_NAMESPACE(detail_);

template<Integer Int>
auto from_chars(const StringView &sv, Int &out, int base /* =10 */) -> Result<void, Error> {
  switch (base) {
  case BaseBin: return detail_::from_chars_<Int, 2> (sv, out);
  case BaseHex: return detail_::from_chars_<Int, 16>(sv, out);
  case BaseDec: return detail_::from_chars_<Int, 10>(sv, out);
  case BaseOct: return detail_::from_chars_<Int, 8> (sv, out);
  default: break;
  }

  return Error{"Invalid numerical base!", ErrC::InvalidArg};
}

template<Integer Int>
auto to_chars(Int in, Span<char> &out, int base /* =10 */) -> Result<usize, Error> {
  switch (base) {
  case BaseBin: return detail_::to_chars_<Int, 2> (in, out);
  case BaseHex: return detail_::to_chars_<Int, 16>(in, out);
  case BaseDec: return detail_::to_chars_<Int, 10>(in, out);
  case BaseOct: return detail_::to_chars_<Int, 8> (in, out);
  default: break;
  }

  return Error{"Invalid numerical base!", ErrC::InvalidArg};
}

END_NAMESPACE_KTA_
