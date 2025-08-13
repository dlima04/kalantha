/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>
#include <Kalantha/Core/Platform.hpp>
#include <Kalantha/Core/Types.hpp>
#include <Kalantha/Core/Memory.hpp>
#include <Kalantha/Meta/Concepts.hpp>
#include <Kalantha/Core/Utility.hpp>
#include <Kalantha/Core/ClassTraits.hpp>
#include <Kalantha/Core/Assertions.hpp>
#include <Kalantha/Core/Result.hpp>
#include <Kalantha/Core/Errors.hpp>
#include <Kalantha/Allocators/AllocatorBase.hpp>
BEGIN_NAMESPACE_KTA_

class BumpAllocator : public AllocatorBase {
  KTA_MAKE_NONCOPYABLE(BumpAllocator);
public:
  template<typename T>
  FORCEINLINE_ auto deallocate_(UNUSED_ T* ptr) -> Result<T, Error> {
    return Error(ErrC::NotImplemented);
  }

  template<kta::TrivialDTOR T, typename ...Args>
  FORCEINLINE_ auto allocate_(Args&&... args) -> T* {
    void* ptr = allocate_block(alignof(T), sizeof(T));
    if(ptr == nullptr) return nullptr;
    return kta::construct_at<T>(ptr, kta::forward<Args>(args)...);
  }

  NODISCARD_ FORCEINLINE_ auto is_valid() const -> bool {
    const bool is_nonnull = beg_ && cur_ && end_;
    const bool rangecheck = end_ >= beg_;
    const bool curcheck   = cur_ >= beg_ && cur_ <= end_;
    return is_nonnull && rangecheck && curcheck;
  }

  NODISCARD_ FORCEINLINE_ auto is_within_range(void* ptr_) const -> bool {
    auto beg = reinterpret_cast<uintptr>(beg_);
    auto end = reinterpret_cast<uintptr>(end_);
    auto ptr = reinterpret_cast<uintptr>(ptr_);
    return ptr >= beg && ptr < end;
  }

  auto allocate_block(usize align, usize size) -> void* {
    usize space = 0;
    auto end = reinterpret_cast<uintptr>(end_);
    auto cur = reinterpret_cast<uintptr>(cur_);
    if(cur < end) space = difference(cur_, end_);

    if(!size || !is_valid() || !space)
      return nullptr;

    void* ptr = checked_align_up(
      align,  /// Use T's natural alignment as the boundary
      size,   /// We need to reserve sizeof(T) bytes.
      cur_,   /// Current pointer position.
      space); /// Space remaining from cur_ up to end_.
    return is_within_range(ptr) ? ptr : nullptr;
  }

  NODISCARD_ auto remaining_() const -> usize {
    auto end = reinterpret_cast<uintptr>(end_);
    auto cur = reinterpret_cast<uintptr>(cur_);
    return cur < end ? difference(cur_, end_) : 0;
  }

  BumpAllocator(BumpAllocator&& other) {
    if(this != &other) {
      this->beg_ = other.beg_;
      this->cur_ = other.cur_;
      this->end_ = other.end_;
      other.beg_ = nullptr;
      other.cur_ = nullptr; /// Clear out other's pointers
      other.end_ = nullptr; ///
    }
  }

  auto operator=(BumpAllocator&& other) -> BumpAllocator& {
    if(this != &other) {
      this->beg_ = other.beg_;
      this->cur_ = other.cur_;
      this->end_ = other.end_;
      other.beg_ = nullptr;
      other.cur_ = nullptr; /// Clear out other's pointers
      other.end_ = nullptr; ///
    }

    return *this;
  }

  BumpAllocator(void* begin, void* end) {
    this->beg_ = begin;
    this->cur_ = begin;
    this->end_ = end;
  }

  NODISCARD_ void* beg() const { return beg_; }
  NODISCARD_ void* cur() const { return cur_; }
  NODISCARD_ void* end() const { return end_; }

  ~BumpAllocator() = default;
  explicit operator bool() const { return is_valid(); }
private:
  void* beg_ = nullptr;
  void* cur_ = nullptr;
  void* end_ = nullptr;
};

END_NAMESPACE_KTA_
