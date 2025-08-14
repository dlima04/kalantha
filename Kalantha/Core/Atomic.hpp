/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

/*
* Note: we assume that the standard intrinsics for atomic memory operations
* are provided by GCC and Clang (__atomic_store, __atomic_load, etc).
*/

#include <Kalantha/Core/Common.hpp>
#include <Kalantha/Core/Types.hpp>
#include <Kalantha/Meta/Concepts.hpp>
#include <Kalantha/Core/Assertions.hpp>
#include <Kalantha/Core/Utility.hpp>
#include <Kalantha/Core/ClassTraits.hpp>
BEGIN_NAMESPACE_KTA_

#define KTA_ATOMIC_RELAXED_ __ATOMIC_RELAXED
#define KTA_ATOMIC_ACQUIRE_ __ATOMIC_ACQUIRE
#define KTA_ATOMIC_RELEASE_ __ATOMIC_RELEASE
#define KTA_ATOMIC_ACQ_REL_ __ATOMIC_ACQ_REL
#define KTA_ATOMIC_SEQ_CST_ __ATOMIC_SEQ_CST

/*
 * MemoryOrder
 * Namespaced memory ordering constants for use by functions that utilize
 * atomic memory operations. These are akin to std::memory_order_* constants in the C++
 * standard library. See more information about memory ordering semantics here:
 * https://en.cppreference.com/w/cpp/atomic/memory_order.html
 */
struct MemoryOrder {
  using Ty_ = decltype(KTA_ATOMIC_RELAXED_);
  static constexpr Ty_ Relaxed = KTA_ATOMIC_RELAXED_;
  static constexpr Ty_ Acquire = KTA_ATOMIC_ACQUIRE_;
  static constexpr Ty_ Release = KTA_ATOMIC_RELEASE_;
  static constexpr Ty_ AcqRel  = KTA_ATOMIC_ACQ_REL_;
  static constexpr Ty_ SeqCst  = KTA_ATOMIC_SEQ_CST_;
  MemoryOrder() = delete;
};

/**
 * @brief Performs an atomic store operation.
 * @param ptr The address of the value to perform the store on.
 * @param val The address of the value to be stored at *ptr.
 * @param order The memory order to use for this operation.
 */
template<typename T>
auto atomic_store(T* ptr, T* val, MemoryOrder::Ty_ order) -> void {
  __atomic_store(ptr, val, order);
}

/**
 * @brief Performs an atomic store operation.
 * @param ptr The address of the value to perform the store on.
 * @param val The value to be stored at ptr.
 * @param order The memory order to use for this operation.
 */
template<typename T>
auto atomic_store(T* ptr, T val, MemoryOrder::Ty_ order) -> void {
  __atomic_store_n(ptr, val, order);
}

/**
 * @brief Performs an atomic load operation
 * @param ptr The address of the value to load.
 * @param out A pointer that receives the loaded value.
 * @param order The memory order to use for this operation.
 */
template<typename T>
auto atomic_load(const T* ptr, T* out, MemoryOrder::Ty_ order) -> void {
  __atomic_load(ptr, out, order);
}

/**
 * @brief Performs an atomic fetch-add operation.
 * @param ptr The address of the value to perform the operation on.
 * @param val The amount to add by.
 * @param order The memory order to use for this operation.
 * @return The old value stored at ptr.
 */
template<typename T>
auto atomic_fetch_add(T* ptr, T val, MemoryOrder::Ty_ order) -> T {
  return __atomic_fetch_add(ptr, val, order);
}

/**
 * @brief Performs an atomic fetch-subtract operation.
 * @param ptr The address of the value to perform the operation on.
 * @param val The amount to subtract by.
 * @param order The memory order to use for this operation.
 * @return The old value stored at ptr.
 */
template<typename T>
auto atomic_fetch_sub(T* ptr, T val, MemoryOrder::Ty_ order) -> T {
  return __atomic_fetch_sub(ptr, val, order);
}

/**
 * @brief Performs an atomic fetch-OR operation.
 * @param ptr The address of the value to perform the operation on.
 * @param val The value to OR by.
 * @param order The memory order to use for this operation.
 * @return The old value stored at ptr.
 */
template<typename T>
auto atomic_fetch_or(T* ptr, T val, MemoryOrder::Ty_ order) -> T {
  return __atomic_fetch_or(ptr, val, order);
}

/**
 * @brief Performs an atomic fetch-AND operation.
 * @param ptr The address of the value to perform the operation on.
 * @param val The value to AND by.
 * @param order The memory order to use for this operation.
 * @return The old value stored at ptr.
 */
template<typename T>
auto atomic_fetch_and(T* ptr, T val, MemoryOrder::Ty_ order) -> T {
  return __atomic_fetch_and(ptr, val, order);
}

/**
 * @brief Performs an atomic fetch-XOR operation.
 * @param ptr The address of the value to perform the operation on.
 * @param val The value to XOR by.
 * @param order The memory order to use for this operation.
 * @return The old value stored at ptr.
 */
template<typename T>
auto atomic_fetch_xor(T* ptr, T val, MemoryOrder::Ty_ order) -> T {
  return __atomic_fetch_xor(ptr, val, order);
}

/**
 * @brief Performs an atomic add-fetch operation.
 * @param ptr The address of the value to perform the operation on.
 * @param val The amount to add by.
 * @param order The memory order to use for this operation.
 * @return The new value stored at ptr.
 */
template<typename T>
auto atomic_add_fetch(T* ptr, T val, MemoryOrder::Ty_ order) -> T {
  return __atomic_add_fetch(ptr, val, order);
}

/**
 * @brief Performs an atomic subtract-fetch operation.
 * @param ptr The address of the value to perform the operation on.
 * @param val The amount to subtract by.
 * @param order The memory order to use for this operation.
 * @return The new value stored at ptr.
 */
template<typename T>
auto atomic_sub_fetch(T* ptr, T val, MemoryOrder::Ty_ order) -> T {
  return __atomic_sub_fetch(ptr, val, order);
}

/**
 * @brief Performs an atomic OR-fetch operation.
 * @param ptr The address of the value to perform the operation on.
 * @param val The value to OR by.
 * @param order The memory order to use for this operation.
 * @return The new value stored at ptr.
 */
template<typename T>
auto atomic_or_fetch(T* ptr, T val, MemoryOrder::Ty_ order) -> T {
  return __atomic_or_fetch(ptr, val, order);
}

/**
 * @brief Performs an atomic AND-fetch operation.
 * @param ptr The address of the value to perform the operation on.
 * @param val The value to AND by.
 * @param order The memory order to use for this operation.
 * @return The new value stored at ptr.
 */
template<typename T>
auto atomic_and_fetch(T* ptr, T val, MemoryOrder::Ty_ order) -> T {
  return __atomic_and_fetch(ptr, val, order);
}

/**
 * @brief Performs an atomic XOR-fetch operation.
 * @param ptr The address of the value to perform the operation on.
 * @param val The value to XOR by.
 * @param order The memory order to use for this operation.
 * @return The new value stored at ptr.
 */
template<typename T>
auto atomic_xor_fetch(T* ptr, T val, MemoryOrder::Ty_ order) -> T {
  return __atomic_xor_fetch(ptr, val, order);
}

/**
 * @brief Performs an atomic exchange operation.
 * @param ptr The address of the value that will be replaced.
 * @param to_exchange_with The address of the value that will be written to ptr.
 * @param order The memory order to use for this operation.
 * @return The old value which was stored at ptr.
 */
template<typename T>
auto atomic_exchange(T* ptr, T* to_exchange_with, MemoryOrder::Ty_ order) -> T {
  return __atomic_exchange_n(ptr, *to_exchange_with, order);
}

/**
 * @brief performs a strong atomic compare-exchange operation.
 * @param location The address of the value to perform the operation on.
 * @param expected The expected value.
 * @param desired If *location == *expected, then *location = desired.
 * @param success_memorder The memory order used when desired is written to *location.
 * @param failure_memorder The memory order used on failure.
 * @return true if the swap succeeded, false if it failed.
 * @note this operation cannot spuriously fail (it's strong).
 */
template<typename T>
auto atomic_compare_exchange_strong(
  T *location,
  T *expected,
  T  desired,
  MemoryOrder::Ty_ success_memorder,
  MemoryOrder::Ty_ failure_memorder ) -> bool
{
  return __atomic_compare_exchange_n(
    location,
    expected,
    desired,
    false,
    success_memorder,
    failure_memorder);
}

/**
 * @brief performs a strong atomic compare-exchange operation.
 * @param location The address of the value to perform the operation on.
 * @param expected The expected value.
 * @param desired If *location == *expected, then *location = desired.
 * @param success_memorder The memory order used when desired is written to *location.
 * @param failure_memorder The memory order used on failure.
 * @return true if the swap succeeded, false if it failed.
 * @warning this operation may spuriously fail. Use with caution.
 */
template<typename T>
auto atomic_compare_exchange_weak(
  T *location,
  T *expected,
  T  desired,
  MemoryOrder::Ty_ success_memorder,
  MemoryOrder::Ty_ failure_memorder ) -> bool
{
  return __atomic_compare_exchange_n(
    location,
    expected,
    desired,
    true,
    success_memorder,
    failure_memorder);
}

/**
 * @brief Clears (zeroes) a specified memory location atomically.
 * @param ptr The address of the memory location to clear.
 * @warning the size of the object is unspecified and therefore this function is unreliable.
*/
auto atomic_clear(volatile void* ptr, MemoryOrder::Ty_ order) -> void {
  __atomic_clear(ptr, order);
}

/**
 * @brief sets a specified memory location to 1.
 * @param ptr The address of the memory location to set.
 * @param order The memory order to use for this operation.
 * @return The old value.
 * @note I think ptr needs to point to a bool or some shit but idk tbh.
 */
auto atomic_test_and_set(volatile void* ptr, MemoryOrder::Ty_ order) -> bool {
  return __atomic_test_and_set(ptr, order);
}

/*
 * AtomicOp
 * Enumeration that explicitly specifies an atomic operation to perform.
 * Used by atomic_fetch_operation and atomic_operation_fetch
 */
enum class AtomicOp : uint8 {
  Add, Sub, Or, And, Xor
};

template<typename T>
auto atomic_fetch_operation(T* ptr, T val, AtomicOp op, MemoryOrder::Ty_ order) -> T {
  switch(op) {
    case AtomicOp::Add: return atomic_fetch_add(ptr, val, order);
    case AtomicOp::Sub: return atomic_fetch_sub(ptr, val, order);
    case AtomicOp::Or:  return atomic_fetch_or(ptr, val, order);
    case AtomicOp::And: return atomic_fetch_and(ptr, val, order);
    case AtomicOp::Xor: return atomic_fetch_xor(ptr, val, order);
    default: break;
  }

  KTA_UNREACHABLE();
}

template<typename T>
auto atomic_operation_fetch(T* ptr, T val, AtomicOp op, MemoryOrder::Ty_ order) -> T {
  switch(op) {
    case AtomicOp::Add: return atomic_add_fetch(ptr, val, order);
    case AtomicOp::Sub: return atomic_sub_fetch(ptr, val, order);
    case AtomicOp::Or:  return atomic_or_fetch(ptr, val, order);
    case AtomicOp::And: return atomic_and_fetch(ptr, val, order);
    case AtomicOp::Xor: return atomic_xor_fetch(ptr, val, order);
    default: break;
  }

  KTA_UNREACHABLE();
}

BEGIN_NAMESPACE(detail_);

template<typename T> requires(IsTriviallyCopyable<T>)
class Atomic_ {
  KTA_MAKE_NONCOPYABLE(Atomic_);
  KTA_MAKE_NONMOVABLE(Atomic_);
public:
  using ValueType = RemoveCV<T>;

  /// @see kta::atomic_load()
  auto load(MemoryOrder::Ty_ order = MemoryOrder::SeqCst) -> T {
    T loaded;
    kta::atomic_load(&val_, &loaded, order);
    return loaded;
  }

  /// @see kta::atomic_store()
  auto store(T to_store, MemoryOrder::Ty_ order = MemoryOrder::SeqCst) -> void {
    kta::atomic_store(&val_, to_store, order);
  }

  /// @see kta::atomic_exchange()
  auto exchange(T& value, MemoryOrder::Ty_ order = MemoryOrder::SeqCst) -> void {
    kta::atomic_exchange(&val_, &value, order);
  }

  /// @see kta::atomic_compare_exchange_weak()
  auto compare_exchange_weak(
    T& expected,
    T  desired,
    MemoryOrder::Ty_ success_memorder = MemoryOrder::SeqCst,
    MemoryOrder::Ty_ failure_memorder = MemoryOrder::SeqCst
  ) -> bool {
    return kta::atomic_compare_exchange_weak(
      &val_,
      &expected,
      desired,
      success_memorder, failure_memorder);
  }

  /// @see kta::atomic_compare_exchange_strong()
  auto compare_exchange_strong(
    T& expected,
    T  desired,
    MemoryOrder::Ty_ success_memorder = MemoryOrder::SeqCst,
    MemoryOrder::Ty_ failure_memorder = MemoryOrder::SeqCst
  ) -> bool {
    return kta::atomic_compare_exchange_strong(
      &val_,
      &expected,
      desired,
      success_memorder, failure_memorder);
  }

  auto& operator=(this auto&& self, T to_store) {
    kta::atomic_store(&self.val_, to_store, MemoryOrder::SeqCst);
    return self;
  }

  constexpr Atomic_(T&& val) : val_(kta::move(val)) {}
  constexpr Atomic_(const T& val) : val_(val) {}
  constexpr Atomic_() = default;
protected:
  T val_{};
};

template<Integer T>
class AtomicInteger : public Atomic_<T> {
  KTA_MAKE_NONCOPYABLE(AtomicInteger);
  KTA_MAKE_NONMOVABLE(AtomicInteger);
public:
  using ValueType = RemoveCV<T>;
  using Atomic_<T>::val_;

#define KTA_ATOMIC_MAKE_METHOD_(KIND) \
  T KIND(T value, MemoryOrder::Ty_ order = MemoryOrder::SeqCst) { \
    return kta :: atomic_##KIND (&val_, value, order); \
  }

  KTA_ATOMIC_MAKE_METHOD_(fetch_add)
  KTA_ATOMIC_MAKE_METHOD_(fetch_sub)
  KTA_ATOMIC_MAKE_METHOD_(fetch_and)
  KTA_ATOMIC_MAKE_METHOD_(fetch_or)
  KTA_ATOMIC_MAKE_METHOD_(fetch_xor)

  KTA_ATOMIC_MAKE_METHOD_(add_fetch)
  KTA_ATOMIC_MAKE_METHOD_(sub_fetch)
  KTA_ATOMIC_MAKE_METHOD_(and_fetch)
  KTA_ATOMIC_MAKE_METHOD_(or_fetch)
  KTA_ATOMIC_MAKE_METHOD_(xor_fetch)

#undef KTA_ATOMIC_MAKE_METHOD_

  /// @see kta::atomic_fetch_operation()
  auto fetch_operation(T val, AtomicOp op, MemoryOrder::Ty_ order = MemoryOrder::SeqCst) -> T {
    switch(op) {
      case AtomicOp::Add: return atomic_fetch_add(&val_, val, order);
      case AtomicOp::Sub: return atomic_fetch_sub(&val_, val, order);
      case AtomicOp::Or:  return atomic_fetch_or (&val_, val, order);
      case AtomicOp::And: return atomic_fetch_and(&val_, val, order);
      case AtomicOp::Xor: return atomic_fetch_xor(&val_, val, order);
      default: break;
    }

    KTA_UNREACHABLE();
  }

  /// @see kta::atomic_operation_fetch()
  auto operation_fetch(T val, AtomicOp op, MemoryOrder::Ty_ order = MemoryOrder::SeqCst) -> T {
    switch(op) {
      case AtomicOp::Add: return atomic_add_fetch(&val_, val, order);
      case AtomicOp::Sub: return atomic_sub_fetch(&val_, val, order);
      case AtomicOp::Or:  return atomic_or_fetch (&val_, val, order);
      case AtomicOp::And: return atomic_and_fetch(&val_, val, order);
      case AtomicOp::Xor: return atomic_xor_fetch(&val_, val, order);
      default: break;
    }

    KTA_UNREACHABLE();
  }

  auto& operator=(this auto&& self, T to_store) {
    kta::atomic_store(&self.val_, to_store, MemoryOrder::SeqCst);
    return self;
  }

  AtomicInteger& operator+=(T val) { fetch_add(val, MemoryOrder::SeqCst); return *this; }
  AtomicInteger& operator-=(T val) { fetch_sub(val, MemoryOrder::SeqCst); return *this; }
  AtomicInteger& operator&=(T val) { fetch_and(val, MemoryOrder::SeqCst); return *this; }
  AtomicInteger& operator|=(T val) { fetch_or (val, MemoryOrder::SeqCst); return *this; }
  AtomicInteger& operator^=(T val) { fetch_xor(val, MemoryOrder::SeqCst); return *this; }

  AtomicInteger& operator++(/*...*/) { fetch_add(1, MemoryOrder::SeqCst); return *this; }
  AtomicInteger& operator++(  int  ) { fetch_add(1, MemoryOrder::SeqCst); return *this; }
  AtomicInteger& operator--(/*...*/) { fetch_sub(1, MemoryOrder::SeqCst); return *this; }
  AtomicInteger& operator--(  int  ) { fetch_sub(1, MemoryOrder::SeqCst); return *this; }

  constexpr AtomicInteger(T val) : Atomic_<T>(val) {}
  constexpr AtomicInteger() = default;
};

template<typename T, bool IsInt_>
struct AtomicDispatcher;

template<typename T>
struct AtomicDispatcher<T, true> {
  using Type = detail_::AtomicInteger<T>;
};

template<typename T>
struct AtomicDispatcher<T, false> {
  using Type = detail_::Atomic_<T>;
};

END_NAMESPACE(detail_);

template<typename T>
using Atomic = typename detail_::AtomicDispatcher<T, IsIntegral<T>>::Type;

using AtomicUInt8   = detail_::AtomicInteger<uint8>;
using AtomicInt8    = detail_::AtomicInteger<int8>;
using AtomicUInt16  = detail_::AtomicInteger<uint16>;
using AtomicInt16   = detail_::AtomicInteger<int16>;
using AtomicUInt32  = detail_::AtomicInteger<uint32>;
using AtomicInt32   = detail_::AtomicInteger<int32>;
using AtomicUInt64  = detail_::AtomicInteger<uint64>;
using AtomicInt64   = detail_::AtomicInteger<int64>;
using AtomicBool    = detail_::AtomicInteger<bool>;
using AtomicByte    = detail_::AtomicInteger<byte>;
using AtomicInt     = detail_::AtomicInteger<int>;
using AtomicUsize   = detail_::AtomicInteger<usize>;
using AtomicVoidPtr = detail_::Atomic_<void*>;
using AtomicPtrDiff = detail_::AtomicInteger<ptrdiff>;

END_NAMESPACE_KTA_
