/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#define KTA_ASSUME_TESTING_ENV_
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>
#include <Kalantha/Core/Atomic.hpp>
#include <Kalantha/Core/Platform.hpp>

#include <thread>
#include <vector>
#include <chrono>
#include <atomic>

using namespace kta;

TEMPLATE_TEST_CASE("atomic_store and atomic_load work correctly", "[atomic_ops]",
                   int, uint32_t, uint64_t) {
  TestType value = TestType{};
  TestType stored_value = static_cast<TestType>(42);
  TestType loaded_value;

  SECTION("Store and load with sequential consistency") {
    atomic_store(&value, stored_value, MemoryOrder::SeqCst);
    atomic_load(&value, &loaded_value, MemoryOrder::SeqCst);
    REQUIRE(loaded_value == stored_value);
  }

  SECTION("Store and load with relaxed ordering") {
    atomic_store(&value, stored_value, MemoryOrder::Relaxed);
    atomic_load(&value, &loaded_value, MemoryOrder::Relaxed);
    REQUIRE(loaded_value == stored_value);
  }

  SECTION("Store with release, load with acquire") {
    atomic_store(&value, stored_value, MemoryOrder::Release);
    atomic_load(&value, &loaded_value, MemoryOrder::Acquire);
    REQUIRE(loaded_value == stored_value);
  }
}

TEST_CASE("idk lol xd", "[Core.Atomic]") {
  REQUIRE(MemoryOrder::Relaxed == __ATOMIC_RELAXED);
  REQUIRE(MemoryOrder::Acquire == __ATOMIC_ACQUIRE);
  REQUIRE(MemoryOrder::Release == __ATOMIC_RELEASE);
  REQUIRE(MemoryOrder::AcqRel == __ATOMIC_ACQ_REL);
  REQUIRE(MemoryOrder::SeqCst == __ATOMIC_SEQ_CST);
}

TEMPLATE_TEST_CASE("atomic fetch operations work correctly", "[atomic_ops]",
                   int, uint32_t, uint64_t) {
  TestType value = static_cast<TestType>(10);
  TestType operand = static_cast<TestType>(5);

  SECTION("fetch_add returns old value and updates") {
    TestType old_val = atomic_fetch_add(&value, operand, MemoryOrder::SeqCst);
    REQUIRE(old_val == static_cast<TestType>(10));
    REQUIRE(value == static_cast<TestType>(15));
  }

  SECTION("fetch_sub returns old value and updates") {
    TestType old_val = atomic_fetch_sub(&value, operand, MemoryOrder::SeqCst);
    REQUIRE(old_val == static_cast<TestType>(10));
    REQUIRE(value == static_cast<TestType>(5));
  }

  SECTION("fetch_or with bitwise operations") {
    value = static_cast<TestType>(0b1010);
    operand = static_cast<TestType>(0b0110);
    TestType old_val = atomic_fetch_or(&value, operand, MemoryOrder::SeqCst);
    REQUIRE(old_val == static_cast<TestType>(0b1010));
    REQUIRE(value == static_cast<TestType>(0b1110));
  }

  SECTION("fetch_and with bitwise operations") {
    value = static_cast<TestType>(0b1110);
    operand = static_cast<TestType>(0b1010);
    TestType old_val = atomic_fetch_and(&value, operand, MemoryOrder::SeqCst);
    REQUIRE(old_val == static_cast<TestType>(0b1110));
    REQUIRE(value == static_cast<TestType>(0b1010));
  }

  SECTION("fetch_xor with bitwise operations") {
    value = static_cast<TestType>(0b1010);
    operand = static_cast<TestType>(0b0110);
    TestType old_val = atomic_fetch_xor(&value, operand, MemoryOrder::SeqCst);
    REQUIRE(old_val == static_cast<TestType>(0b1010));
    REQUIRE(value == static_cast<TestType>(0b1100));
  }
}

TEMPLATE_TEST_CASE("atomic operation_fetch works correctly", "[atomic_ops]",
                   int, uint32_t, uint64_t) {
  TestType value = static_cast<TestType>(10);
  TestType operand = static_cast<TestType>(5);

  SECTION("add_fetch returns new value") {
    TestType new_val = atomic_add_fetch(&value, operand, MemoryOrder::SeqCst);
    REQUIRE(new_val == static_cast<TestType>(15));
    REQUIRE(value == static_cast<TestType>(15));
  }

  SECTION("sub_fetch returns new value") {
    TestType new_val = atomic_sub_fetch(&value, operand, MemoryOrder::SeqCst);
    REQUIRE(new_val == static_cast<TestType>(5));
    REQUIRE(value == static_cast<TestType>(5));
  }

  SECTION("or_fetch returns new value") {
    value = static_cast<TestType>(0b1010);
    operand = static_cast<TestType>(0b0110);
    TestType new_val = atomic_or_fetch(&value, operand, MemoryOrder::SeqCst);
    REQUIRE(new_val == static_cast<TestType>(0b1110));
    REQUIRE(value == static_cast<TestType>(0b1110));
  }

  SECTION("and_fetch returns new value") {
    value = static_cast<TestType>(0b1110);
    operand = static_cast<TestType>(0b1010);
    TestType new_val = atomic_and_fetch(&value, operand, MemoryOrder::SeqCst);
    REQUIRE(new_val == static_cast<TestType>(0b1010));
    REQUIRE(value == static_cast<TestType>(0b1010));
  }

  SECTION("xor_fetch returns new value") {
    value = static_cast<TestType>(0b1010);
    operand = static_cast<TestType>(0b0110);
    TestType new_val = atomic_xor_fetch(&value, operand, MemoryOrder::SeqCst);
    REQUIRE(new_val == static_cast<TestType>(0b1100));
    REQUIRE(value == static_cast<TestType>(0b1100));
  }
}

TEST_CASE("atomic_exchange works correctly", "[atomic_ops]") {
  int value = 42;
  int to_exchange = 100;

  int old_val = atomic_exchange(&value, &to_exchange, MemoryOrder::SeqCst);

  REQUIRE(old_val == 42);
  REQUIRE(value == 100);
}

TEMPLATE_TEST_CASE("atomic_compare_exchange_strong works correctly", "[atomic_ops]",
                   int, uint32_t, uint64_t) {
  TestType value = static_cast<TestType>(42);
  TestType expected = static_cast<TestType>(42);
  TestType desired = static_cast<TestType>(100);

  SECTION("Successful exchange when expected matches") {
    bool success = atomic_compare_exchange_strong(
      &value, &expected, desired,
      MemoryOrder::SeqCst, MemoryOrder::SeqCst);

    REQUIRE(success == true);
    REQUIRE(value == desired);
    REQUIRE(expected == static_cast<TestType>(42)); // Should remain unchanged on success
  }

  SECTION("Failed exchange when expected doesn't match") {
    value = static_cast<TestType>(50); // Different from expected
    expected = static_cast<TestType>(42);

    bool success = atomic_compare_exchange_strong(
      &value, &expected, desired,
      MemoryOrder::SeqCst, MemoryOrder::SeqCst);

    REQUIRE(success == false);
    REQUIRE(value == static_cast<TestType>(50)); // Should remain unchanged
    REQUIRE(expected == static_cast<TestType>(50)); // Should be updated with actual value
  }
}

TEMPLATE_TEST_CASE("atomic_compare_exchange_weak works correctly", "[atomic_ops]",
                   int, uint32_t, uint64_t) {
  TestType value = static_cast<TestType>(42);
  TestType expected = static_cast<TestType>(42);
  TestType desired = static_cast<TestType>(100);

  SECTION("Successful exchange when expected matches") {
    // Weak CAS might spuriously fail, so we loop until success
    bool success = false;
    int attempts = 0;
    while (!success && attempts < 10) {
      expected = static_cast<TestType>(42); // Reset expected value
      success = atomic_compare_exchange_weak(
        &value, &expected, desired,
        MemoryOrder::SeqCst, MemoryOrder::SeqCst);
      attempts++;
    }

    REQUIRE(success == true);
    REQUIRE(value == desired);
  }
}

TEST_CASE("atomic_test_and_set", "[Core.Atomic]") {
  volatile bool flag = false;
  bool old_value = atomic_test_and_set(&flag, MemoryOrder::SeqCst);
  REQUIRE(old_value == false); // Was initially false
  REQUIRE(flag == true); // Now should be true
}

TEST_CASE("AtomicOp enumeration and helper functions", "[atomic_op]") {
  int value = 10;
  int operand = 5;

  SECTION("atomic_fetch_operation with Add") {
    int old_val = atomic_fetch_operation(&value, operand, AtomicOp::Add, MemoryOrder::SeqCst);
    REQUIRE(old_val == 10);
    REQUIRE(value == 15);
  }

  SECTION("atomic_fetch_operation with Sub") {
    value = 10;
    int old_val = atomic_fetch_operation(&value, operand, AtomicOp::Sub, MemoryOrder::SeqCst);
    REQUIRE(old_val == 10);
    REQUIRE(value == 5);
  }

  SECTION("atomic_operation_fetch with Add") {
    value = 10;
    int new_val = atomic_operation_fetch(&value, operand, AtomicOp::Add, MemoryOrder::SeqCst);
    REQUIRE(new_val == 15);
    REQUIRE(value == 15);
  }
}

TEMPLATE_TEST_CASE("Atomic<T> class basic operations", "[atomic_class]",
                   int, uint64_t) {
  Atomic<TestType> atomic_val{};

  SECTION("Construction and assignment") {
    TestType test_value = static_cast<TestType>(42);
    Atomic<TestType> atomic_constructed(test_value);

    TestType loaded = atomic_constructed.load();
    REQUIRE(loaded == test_value);
  }

  SECTION("Store and load") {
    TestType test_value = static_cast<TestType>(123);
    atomic_val.store(test_value);
    TestType loaded = atomic_val.load();
    REQUIRE(loaded == test_value);
  }

  SECTION("Exchange") {
    TestType initial_value = static_cast<TestType>(50);
    TestType exchange_value = static_cast<TestType>(75);

    atomic_val.store(initial_value);
    atomic_val.exchange(exchange_value);
    TestType loaded = atomic_val.load();
    REQUIRE(loaded == exchange_value);
  }

  SECTION("Compare exchange strong") {
    TestType initial_value = static_cast<TestType>(100);
    TestType expected = static_cast<TestType>(100);
    TestType desired = static_cast<TestType>(200);

    atomic_val.store(initial_value);
    bool success = atomic_val.compare_exchange_strong(expected, desired);

    REQUIRE(success == true);
    REQUIRE(atomic_val.load() == desired);
  }

  SECTION("Assignment operator") {
    TestType test_value = static_cast<TestType>(300);
    atomic_val = test_value;
    REQUIRE(atomic_val.load() == test_value);
  }
}

TEMPLATE_TEST_CASE("AtomicInteger class operations", "[atomic_integer]",
                   int, uint32_t, uint64_t) {
  Atomic<TestType> atomic_int(static_cast<TestType>(10));

  SECTION("Fetch-add operations") {
    TestType old_val = atomic_int.fetch_add(static_cast<TestType>(5));
    REQUIRE(old_val == static_cast<TestType>(10));
    REQUIRE(atomic_int.load() == static_cast<TestType>(15));
  }

  SECTION("Add-fetch operations") {
    atomic_int.store(static_cast<TestType>(10));
    TestType new_val = atomic_int.add_fetch(static_cast<TestType>(5));
    REQUIRE(new_val == static_cast<TestType>(15));
    REQUIRE(atomic_int.load() == static_cast<TestType>(15));
  }

  SECTION("Compound assignment operators") {
    atomic_int.store(static_cast<TestType>(10));
    atomic_int += static_cast<TestType>(5);
    REQUIRE(atomic_int.load() == static_cast<TestType>(15));

    atomic_int -= static_cast<TestType>(3);
    REQUIRE(atomic_int.load() == static_cast<TestType>(12));

    atomic_int &= static_cast<TestType>(0b1010);
    REQUIRE(atomic_int.load() == static_cast<TestType>(8)); // 12 & 10 = 8

    atomic_int |= static_cast<TestType>(0b0101);
    REQUIRE(atomic_int.load() == static_cast<TestType>(13)); // 8 | 5 = 13
    atomic_int ^= static_cast<TestType>(0b1111);
    REQUIRE(atomic_int.load() == static_cast<TestType>(2)); // 13 ^ 15 = 2
  }

  SECTION("Increment and decrement operators") {
    atomic_int.store(static_cast<TestType>(5));

    ++atomic_int;
    REQUIRE(atomic_int.load() == static_cast<TestType>(6));

    atomic_int++;
    REQUIRE(atomic_int.load() == static_cast<TestType>(7));

    --atomic_int;
    REQUIRE(atomic_int.load() == static_cast<TestType>(6));

    atomic_int--;
    REQUIRE(atomic_int.load() == static_cast<TestType>(5));
  }

  SECTION("fetch_operation and operation_fetch methods") {
    atomic_int.store(static_cast<TestType>(20));

    TestType old_val = atomic_int.fetch_operation(static_cast<TestType>(5), AtomicOp::Sub);
    REQUIRE(old_val == static_cast<TestType>(20));
    REQUIRE(atomic_int.load() == static_cast<TestType>(15));

    TestType new_val = atomic_int.operation_fetch(static_cast<TestType>(3), AtomicOp::Add);
    REQUIRE(new_val == static_cast<TestType>(18));
    REQUIRE(atomic_int.load() == static_cast<TestType>(18));
  }
}

TEST_CASE("Multithreading stuff", "[Core.Atomic]") {
  const int num_threads = 4;
  const int operations_per_thread = 1000;
  AtomicInt32 counter(0);

  SECTION("Concurrent increments") {
    std::vector<std::thread> threads;

#  if KTA_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-lambda-capture"
#  endif

    for (int i = 0; i < num_threads; ++i) {
      threads.emplace_back([&counter, operations_per_thread]() {
        for (int j = 0; j < operations_per_thread; ++j) {
          counter.fetch_add(1);
        }
      });
    }

#  if KTA_CLANG
#pragma clang diagnostic pop
#  endif

    for (auto& t : threads) {
      t.join();
    }

    REQUIRE(counter.load() == num_threads * operations_per_thread);
  }
}

TEST_CASE("Weirdness", "[Core.Atomic]") {
  SECTION("Overflow behavior") {
    AtomicUInt8 atomic_overflow(255);
    atomic_overflow.fetch_add(1);
    REQUIRE(atomic_overflow.load() == 0); // Should wrap around
  }
}
