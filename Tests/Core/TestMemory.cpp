/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#define KTA_ASSUME_TESTING_ENV_
#include <catch2/catch_test_macros.hpp>
#include <Kalantha/Core/Memory.hpp>

using namespace kta;

TEST_CASE("align_up function tests", "[Core.Memory.Util]") {
  SECTION("Invalid alignment values") {
    char buffer[100];
    void* ptr = buffer;

    // Zero alignment
    REQUIRE(align_up(0, ptr) == nullptr);

    // Non-power-of-2 alignments
    REQUIRE(align_up(3, ptr) == nullptr);
    REQUIRE(align_up(5, ptr) == nullptr);
    REQUIRE(align_up(6, ptr) == nullptr);
    REQUIRE(align_up(7, ptr) == nullptr);
    REQUIRE(align_up(9, ptr) == nullptr);
  }

  SECTION("Valid power-of-2 alignments") {
    char buffer[100];

    // Test with alignment of 1 (should return same pointer)
    void* ptr1 = buffer;
    void* aligned1 = align_up(1, ptr1);
    REQUIRE(aligned1 == ptr1);

    // Test with alignment of 2
    void* ptr2 = buffer + 1;
    void* aligned2 = align_up(2, ptr2);
    uintptr addr2 = reinterpret_cast<uintptr>(aligned2);
    REQUIRE(addr2 % 2 == 0);
    REQUIRE(aligned2 >= ptr2);

    // Test with alignment of 4
    void* ptr4 = buffer + 1;
    void* aligned4 = align_up(4, ptr4);
    uintptr addr4 = reinterpret_cast<uintptr>(aligned4);
    REQUIRE(addr4 % 4 == 0);
    REQUIRE(aligned4 >= ptr4);

    // Test with alignment of 8
    void* ptr8 = buffer + 3;
    void* aligned8 = align_up(8, ptr8);
    uintptr addr8 = reinterpret_cast<uintptr>(aligned8);
    REQUIRE(addr8 % 8 == 0);
    REQUIRE(aligned8 >= ptr8);
  }

  SECTION("Already aligned pointers") {
    alignas(16) char buffer[100];
    void* ptr = buffer;

    // Should return same pointer if already aligned
    REQUIRE(align_up(16, ptr) == ptr);
    REQUIRE(align_up(8, ptr) == ptr);
    REQUIRE(align_up(4, ptr) == ptr);
  }

  SECTION("Null pointer handling") {
    void* null_ptr = nullptr;

    // Should handle null pointers gracefully
    REQUIRE(align_up(4, null_ptr) == nullptr);
    REQUIRE(align_up(8, null_ptr) == nullptr);
  }
}

TEST_CASE("align_down function tests", "[Core.Memory.Util]") {
  SECTION("Invalid alignment values") {
    char buffer[100];
    void* ptr = buffer;

    // Zero alignment
    REQUIRE(align_down(0, ptr) == nullptr);

    // Non-power-of-2 alignments
    REQUIRE(align_down(3, ptr) == nullptr);
    REQUIRE(align_down(5, ptr) == nullptr);
    REQUIRE(align_down(6, ptr) == nullptr);
    REQUIRE(align_down(7, ptr) == nullptr);
    REQUIRE(align_down(9, ptr) == nullptr);
  }

  SECTION("Valid power-of-2 alignments") {
    char buffer[100];

    // Test with alignment of 1 (should return same pointer)
    void* ptr1 = buffer + 5;
    void* aligned1 = align_down(1, ptr1);
    REQUIRE(aligned1 == ptr1);

    // Test with alignment of 2
    void* ptr2 = buffer + 7;
    void* aligned2 = align_down(2, ptr2);
    uintptr addr2 = reinterpret_cast<uintptr>(aligned2);
    REQUIRE(addr2 % 2 == 0);
    REQUIRE(aligned2 <= ptr2);

    // Test with alignment of 4
    void* ptr4 = buffer + 9;
    void* aligned4 = align_down(4, ptr4);
    uintptr addr4 = reinterpret_cast<uintptr>(aligned4);
    REQUIRE(addr4 % 4 == 0);
    REQUIRE(aligned4 <= ptr4);

    // Test with alignment of 8
    void* ptr8 = buffer + 15;
    void* aligned8 = align_down(8, ptr8);
    uintptr addr8 = reinterpret_cast<uintptr>(aligned8);
    REQUIRE(addr8 % 8 == 0);
    REQUIRE(aligned8 <= ptr8);
  }

  SECTION("Already aligned pointers") {
    alignas(16) char buffer[100];
    void* ptr = buffer;

    // Should return same pointer if already aligned
    REQUIRE(align_down(16, ptr) == ptr);
    REQUIRE(align_down(8, ptr) == ptr);
    REQUIRE(align_down(4, ptr) == ptr);
  }

  SECTION("Null pointer handling") {
    void* null_ptr = nullptr;

    // Should handle null pointers gracefully
    REQUIRE(align_down(4, null_ptr) == nullptr);
    REQUIRE(align_down(8, null_ptr) == nullptr);
  }
}

TEST_CASE("Checked_align_up function tests", "[Core.Memory.Util]") {
  SECTION("Invalid alignment values") {
    char buffer[100];
    void* ptr = buffer;
    usize space = 100;

    // Non-power-of-2 alignments
    REQUIRE(checked_align_up(3, 4, ptr, space) == nullptr);
    REQUIRE(checked_align_up(5, 4, ptr, space) == nullptr);
    REQUIRE(checked_align_up(6, 4, ptr, space) == nullptr);
    REQUIRE(checked_align_up(7, 4, ptr, space) == nullptr);
    REQUIRE(checked_align_up(9, 4, ptr, space) == nullptr);
  }

  SECTION("Insufficient space") {
    char buffer[100];
    void* ptr = buffer;
    usize space = 4;

    // Size greater than space
    REQUIRE(checked_align_up(4, 8, ptr, space) == nullptr);

    // Size equal to space should work
    void* result = checked_align_up(4, 4, ptr, space);
    REQUIRE(result != nullptr);
  }

  SECTION("Successful alignment and space management") {
    char buffer[100];
    void* original_ptr = buffer + 1;  // Misaligned pointer
    void* ptr = original_ptr;
    usize space = 50;
    usize original_space = space;

    void* aligned = checked_align_up(8, 16, ptr, space);

    REQUIRE(aligned != nullptr);

    // Check alignment
    uintptr addr = reinterpret_cast<uintptr>(aligned);
    REQUIRE(addr % 8 == 0);

    // Check that ptr was updated correctly
    REQUIRE(ptr == reinterpret_cast<void*>(reinterpret_cast<uintptr>(aligned) + 16));

    // Check space was updated correctly
    uintptr alignment_offset = addr - reinterpret_cast<uintptr>(original_ptr);
    REQUIRE(space == original_space - alignment_offset - 16);
  }

  SECTION("Edge case: exact space match") {
    alignas(8) char buffer[100];
    void* ptr = buffer;
    usize space = 16;

    void* aligned = checked_align_up(8, 16, ptr, space);

    REQUIRE(aligned != nullptr);
    REQUIRE(space == 0);  // All space should be consumed
  }

  SECTION("Edge case: alignment causes overflow") {
    char buffer[100];
    void* ptr = buffer + 90;  // Near end of buffer
    usize space = 8;

    // Alignment might push us beyond available space
    void* aligned = checked_align_up(16, 8, ptr, space);

    // This might be nullptr if alignment pushes beyond space
    if (aligned != nullptr) {
      uintptr addr = reinterpret_cast<uintptr>(aligned);
      REQUIRE(addr % 16 == 0);
    }
  }
}

TEST_CASE("Difference function tests", "[Core.Memory.Util]") {
  SECTION("Basic pointer arithmetic") {
    char buffer[100];
    void* start = buffer;
    void* end = buffer + 50;

    REQUIRE(difference(start, end) == 50);
    REQUIRE(difference(end, start) == static_cast<usize>(-50));  // Underflow behavior
  }

  SECTION("Same pointer") {
    char buffer[100];
    void* ptr = buffer;

    REQUIRE(difference(ptr, ptr) == 0);
  }

  SECTION("Different data types") {
    int array[10];
    void* start = array;
    void* end = array + 5;

    REQUIRE(difference(start, end) == 5 * sizeof(int));
  }

  SECTION("Null pointers") {
    void* null_ptr = nullptr;
    char buffer[100];
    void* ptr = buffer;

    REQUIRE(difference(null_ptr, null_ptr) == 0);
    REQUIRE(difference(null_ptr, ptr) == reinterpret_cast<uintptr>(ptr));
    REQUIRE(difference(ptr, null_ptr) == static_cast<usize>(-reinterpret_cast<uintptr>(ptr)));
  }
}

TEST_CASE("Consistency", "[Core.Memory.Util]") {
  SECTION("align_up and align_down consistency") {
    char buffer[100];
    void* ptr = buffer + 13;  // Arbitrary offset

    void* up_aligned = align_up(8, ptr);
    void* down_aligned = align_down(8, ptr);

    REQUIRE(up_aligned != nullptr);
    REQUIRE(down_aligned != nullptr);
    REQUIRE(up_aligned >= ptr);
    REQUIRE(down_aligned <= ptr);

    // The difference should be less than or equal to alignment
    usize diff = difference(down_aligned, up_aligned);
    REQUIRE(diff <= 8);
  }

  SECTION("checked_align_up with multiple allocations") {
    char buffer[1000];
    void* ptr = buffer;
    usize space = 1000;

    void* alloc1 = checked_align_up(8, 100, ptr, space);
    REQUIRE(alloc1 != nullptr);

    void* alloc2 = checked_align_up(16, 200, ptr, space);
    REQUIRE(alloc2 != nullptr);

    void* alloc3 = checked_align_up(32, 300, ptr, space);
    REQUIRE(alloc3 != nullptr);

    REQUIRE(difference(alloc1, alloc2) >= 100);
    REQUIRE(difference(alloc2, alloc3) >= 200);
  }
}

