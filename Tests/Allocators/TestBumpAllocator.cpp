/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#define KTA_ASSUME_TESTING_ENV_
#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <Kalantha/Allocators/BumpAllocator.hpp>

#include <vector>
#include <memory>
#include <cstdint>
#include <cstring>

using namespace kta;

/// Test fixture for BumpAllocator tests
class BumpAllocatorFixture {
public:
  static constexpr size_t BUFFER_SIZE = 1024;

  BumpAllocatorFixture() {
    buffer = std::make_unique<uint8_t[]>(BUFFER_SIZE);
    buffer_start = buffer.get();
    buffer_end = buffer_start + BUFFER_SIZE;
  }

  ~BumpAllocatorFixture() = default;

  std::unique_ptr<uint8_t[]> buffer;
  uint8_t* buffer_start;
  uint8_t* buffer_end;
};

// Simple test structures
struct TestStruct {
  int value = 0;
  double data = 0.0;

  TestStruct() = default;
  TestStruct(int v, double d) : value(v), data(d) {}
};

struct alignas(16) AlignedStruct {
  int value = 0;
  AlignedStruct() = default;
  AlignedStruct(int v) : value(v) {}
};

TEST_CASE_METHOD(BumpAllocatorFixture, "BumpAllocator - Construction and Basic State", "[Core.Memory.BumpAllocator]") {
  SECTION("Constructor with valid range") {
    BumpAllocator allocator(buffer_start, buffer_end);
    
    REQUIRE(allocator.is_valid());
    REQUIRE(allocator.beg() == static_cast<void*>(buffer_start));
    REQUIRE(allocator.cur() == static_cast<void*>(buffer_start));
    REQUIRE(allocator.end() == static_cast<void*>(buffer_end));
    REQUIRE(allocator.is_valid());
  }
  
  SECTION("Constructor with invalid range (end < begin)") {
    BumpAllocator allocator(buffer_end, buffer_start);
    
    REQUIRE_FALSE(allocator.is_valid());
    REQUIRE_FALSE(static_cast<bool>(allocator));
  }
  
  SECTION("Constructor with null pointers") {
    BumpAllocator allocator(nullptr, nullptr);
    
    REQUIRE_FALSE(allocator.is_valid());
    REQUIRE_FALSE(static_cast<bool>(allocator));
  }
  
  SECTION("Constructor with same begin and end") {
    BumpAllocator allocator(buffer_start, buffer_start);
    
    REQUIRE(allocator.is_valid());
    REQUIRE(allocator.remaining_() == 0);
  }
}

TEST_CASE_METHOD(BumpAllocatorFixture, "BumpAllocator - Move Semantics", "[Core.Memory.BumpAllocator]") {
  SECTION("Move constructor") {
    BumpAllocator original(buffer_start, buffer_end);
    
    // Allocate something to change the current pointer
    int* ptr = original.allocate_<int>(42);
    REQUIRE(ptr != nullptr);
    
    void* original_cur = original.cur();
    
    BumpAllocator moved(std::move(original));
    
    // Check moved allocator has correct state
    REQUIRE(moved.is_valid());
    REQUIRE(moved.beg() == buffer_start);
    REQUIRE(moved.cur() == original_cur);
    REQUIRE(moved.end() == buffer_end);
    
    // Check original allocator is cleared
    REQUIRE_FALSE(original.is_valid());
    REQUIRE(original.beg() == nullptr);
    REQUIRE(original.cur() == nullptr);
    REQUIRE(original.end() == nullptr);
  }
  
  SECTION("Move assignment") {
    BumpAllocator original(buffer_start, buffer_end);
    BumpAllocator target(nullptr, nullptr);
    
    // Allocate something to change the current pointer
    int* ptr = original.allocate_<int>(42);
    REQUIRE(ptr != nullptr);
    
    void* original_cur = original.cur();
    
    target = std::move(original);
    
    // Check target allocator has correct state
    REQUIRE(target.is_valid());
    REQUIRE(target.beg() == buffer_start);
    REQUIRE(target.cur() == original_cur);
    REQUIRE(target.end() == buffer_end);
    
    // Check original allocator is cleared
    REQUIRE_FALSE(original.is_valid());
    REQUIRE(original.beg() == nullptr);
    REQUIRE(original.cur() == nullptr);
    REQUIRE(original.end() == nullptr);
  }
  
  SECTION("Self-assignment") {
    BumpAllocator allocator(buffer_start, buffer_end);
    
    int* ptr = allocator.allocate_<int>(42);
    REQUIRE(ptr != nullptr);
    void* cur_before = allocator.cur();

#  if KTA_CLANG

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wself-move"

#  elif KTA_GCC

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-move"

#  endif

    allocator = std::move(allocator);

#  if KTA_CLANG
#pragma clang diagnostic pop

#  elif KTA_GCC
#pragma GCC diagnostic pop

#  endif

    // Should remain unchanged
    REQUIRE(allocator.is_valid());
    REQUIRE(allocator.beg() == buffer_start);
    REQUIRE(allocator.cur() == cur_before);
    REQUIRE(allocator.end() == buffer_end);
  }
}

TEST_CASE_METHOD(BumpAllocatorFixture, "BumpAllocator - Basic Allocation", "[Core.Memory.BumpAllocator]") {
  BumpAllocator allocator(buffer_start, buffer_end);
  
  SECTION("Allocate single int") {
    int* ptr = allocator.allocate_<int>(42);
    
    REQUIRE(ptr != nullptr);
    REQUIRE(*ptr == 42);
    REQUIRE(allocator.is_within_range(ptr));
    REQUIRE(allocator.cur() > allocator.beg());
  }
  
  SECTION("Allocate multiple objects") {
    int* ptr1 = allocator.allocate_<int>(1);
    int* ptr2 = allocator.allocate_<int>(2);
    int* ptr3 = allocator.allocate_<int>(3);
    
    REQUIRE(ptr1 != nullptr);
    REQUIRE(ptr2 != nullptr);
    REQUIRE(ptr3 != nullptr);
    
    REQUIRE(*ptr1 == 1);
    REQUIRE(*ptr2 == 2);
    REQUIRE(*ptr3 == 3);
    
    REQUIRE(allocator.is_within_range(ptr1));
    REQUIRE(allocator.is_within_range(ptr2));
    REQUIRE(allocator.is_within_range(ptr3));
    
    // Check pointers are in order
    REQUIRE(ptr1 < ptr2);
    REQUIRE(ptr2 < ptr3);
  }
  
  SECTION("Allocate custom struct") {
    TestStruct* ptr = allocator.allocate_<TestStruct>(100, 3.14);
    
    REQUIRE(ptr != nullptr);
    REQUIRE(ptr->value == 100);
    REQUIRE(ptr->data == Catch::Approx(3.14));
    REQUIRE(allocator.is_within_range(ptr));
  }
  
  SECTION("Allocate aligned struct") {
    AlignedStruct* ptr = allocator.allocate_<AlignedStruct>(42);
    
    REQUIRE(ptr != nullptr);
    REQUIRE(ptr->value == 42);
    REQUIRE(allocator.is_within_range(ptr));
    
    // Check alignment
    uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    REQUIRE(addr % 16 == 0);
  }
}

TEST_CASE_METHOD(BumpAllocatorFixture, "BumpAllocator - Allocation Limits", "[Core.Memory.BumpAllocator]") {
  // Use a smaller buffer for these tests
  constexpr size_t SMALL_SIZE = 32;
  auto small_buffer = std::make_unique<uint8_t[]>(SMALL_SIZE);
  BumpAllocator allocator(small_buffer.get(), small_buffer.get() + SMALL_SIZE);
  
  SECTION("Fill buffer completely") {
    std::vector<uint8_t*> ptrs;
    
    // Allocate until we can't anymore
    for (size_t i = 0; i < SMALL_SIZE; ++i) {
      uint8_t* ptr = allocator.allocate_<uint8_t>(static_cast<uint8_t>(i));
      if (ptr == nullptr) break;
      ptrs.push_back(ptr);
    }
    
    REQUIRE_FALSE(ptrs.empty());
    REQUIRE(allocator.remaining_() < sizeof(uint8_t));
    
    // Try to allocate one more - should fail
    uint8_t* ptr = allocator.allocate_<uint8_t>(99);
    REQUIRE(ptr == nullptr);
  }
  
  SECTION("Allocate object larger than remaining space") {
    // Fill most of the buffer
    size_t bytes_to_fill = SMALL_SIZE - sizeof(int) + 1;
    for (size_t i = 0; i < bytes_to_fill; ++i) {
      allocator.allocate_<uint8_t>(0);
    }

    // Try to allocate an int - should fail
    int* ptr = allocator.allocate<int>(42);
    REQUIRE(ptr == nullptr);
  }
}

TEST_CASE_METHOD(BumpAllocatorFixture, "BumpAllocator - Range Checking", "[Core.Memory.BumpAllocator]") {
  BumpAllocator allocator(buffer_start, buffer_end);
  
  SECTION("is_within_range with valid pointers") {
    int* ptr = allocator.allocate_<int>(42);
    REQUIRE(ptr != nullptr);
    
    REQUIRE(allocator.is_within_range(ptr));
    REQUIRE(allocator.is_within_range(buffer_start));
    REQUIRE_FALSE(allocator.is_within_range(buffer_end)); // End is exclusive
  }
  
  SECTION("is_within_range with invalid pointers") {
    // Create another buffer outside our range
    auto other_buffer = std::make_unique<uint8_t[]>(100);
    
    REQUIRE_FALSE(allocator.is_within_range(other_buffer.get()));
    REQUIRE_FALSE(allocator.is_within_range(nullptr));
    
    // Test pointer before range
    if (buffer_start > reinterpret_cast<uint8_t*>(0x1000)) {
      REQUIRE_FALSE(allocator.is_within_range(buffer_start - 1));
    }
    
    // Test pointer after range
    REQUIRE_FALSE(allocator.is_within_range(buffer_end + 1));
  }
}

TEST_CASE_METHOD(BumpAllocatorFixture, "BumpAllocator - Remaining Space", "[Core.Memory.BumpAllocator]") {
  BumpAllocator allocator(buffer_start, buffer_end);
  
  SECTION("Initial remaining space") {
    REQUIRE(allocator.remaining_() == BUFFER_SIZE);
  }
  
  SECTION("Remaining space after allocations") {
    size_t initial_remaining = allocator.remaining_();
    
    int* ptr = allocator.allocate_<int>(42);
    REQUIRE(ptr != nullptr);
    
    size_t remaining_after = allocator.remaining_();
    REQUIRE(remaining_after < initial_remaining);

    auto cur_ptr = reinterpret_cast<uintptr_t>(allocator.cur());
    auto beg_ptr = reinterpret_cast<uintptr_t>(allocator.beg());

    REQUIRE(remaining_after == initial_remaining - (cur_ptr - beg_ptr));
  }
  
  SECTION("Zero remaining space") {
    // Use a very small buffer
    constexpr size_t TINY_SIZE = sizeof(int);
    auto tiny_buffer = std::make_unique<uint8_t[]>(TINY_SIZE);
    BumpAllocator tiny_allocator(tiny_buffer.get(), tiny_buffer.get() + TINY_SIZE);
    
    int* ptr = tiny_allocator.allocate_<int>(42);
    REQUIRE(ptr != nullptr);
    
    REQUIRE(tiny_allocator.remaining_() == 0);
  }
}

TEST_CASE_METHOD(BumpAllocatorFixture, "BumpAllocator - Deallocation", "[Core.Memory.BumpAllocator]") {
  BumpAllocator allocator(buffer_start, buffer_end);
  
  SECTION("Deallocate returns not implemented error") {
    int* ptr = allocator.allocate_<int>(42);
    REQUIRE(ptr != nullptr);
    
    auto result = allocator.deallocate_(ptr);
    REQUIRE_FALSE(result.has_value());
    REQUIRE(result.error().code == ErrC::NotImplemented);
  }
}

TEST_CASE_METHOD(BumpAllocatorFixture, "BumpAllocator - Invalid States", "[Core.Memory.BumpAllocator]") {
  SECTION("Allocation from invalid allocator") {
    BumpAllocator allocator(nullptr, nullptr);
    
    int* ptr = allocator.allocate_<int>(42);
    REQUIRE(ptr == nullptr);
  }
  
  SECTION("Operations on moved-from allocator") {
    BumpAllocator allocator(buffer_start, buffer_end);
    BumpAllocator moved_to(std::move(allocator));
    
    // Original allocator should be invalid
    REQUIRE_FALSE(allocator.is_valid());
    
    int* ptr = allocator.allocate_<int>(42);
    REQUIRE(ptr == nullptr);
    
    REQUIRE(allocator.remaining_() == 0);
  }
}

TEST_CASE_METHOD(BumpAllocatorFixture, "BumpAllocator - Alignment Requirements", "[Core.Memory.BumpAllocator]") {
  BumpAllocator allocator(buffer_start, buffer_end);
  
  SECTION("Natural alignment for basic types") {
    char* char_ptr = allocator.allocate_<char>('a');
    REQUIRE(char_ptr != nullptr);
    
    int* int_ptr = allocator.allocate_<int>(42);
    REQUIRE(int_ptr != nullptr);
    
    double* double_ptr = allocator.allocate_<double>(3.14);
    REQUIRE(double_ptr != nullptr);
    
    // Check alignment
    REQUIRE(reinterpret_cast<uintptr_t>(int_ptr) % alignof(int) == 0);
    REQUIRE(reinterpret_cast<uintptr_t>(double_ptr) % alignof(double) == 0);
  }
  
  SECTION("Custom alignment") {
    AlignedStruct* ptr = allocator.allocate_<AlignedStruct>(42);
    REQUIRE(ptr != nullptr);
    
    uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    REQUIRE(addr % alignof(AlignedStruct) == 0);
    REQUIRE(addr % 16 == 0);
  }
}
