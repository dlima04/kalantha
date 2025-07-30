/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#define KTA_ASSUME_TESTING_ENV_
#include <catch2/catch_test_macros.hpp>
#include <Kalantha/Core/OStream.hpp>
#include <Kalantha/Core/Assertions.hpp>

#include <string>
#include <vector>
#include <iostream>

using namespace kta;

static void write_handler([[maybe_unused]] StringView buf) {
  // can write to stdout or whatever
  std::cout.write(buf.data(), buf.size());
}

TEST_CASE("BasicFunctionality", "[Core.OStream]") {
  SECTION("Initialization") {
    // Test default initialization
    OStream<> stream(write_handler);

    REQUIRE(stream.buffer_max_size()  == KTA_OSTREAM_BUFSIZE_);
    REQUIRE(stream.buffer_current()   == 0);
    REQUIRE(stream.buffer_remaining() == KTA_OSTREAM_BUFSIZE_);
  }

  SECTION("WriteSmallData") {
    // Test writing small data that fits in the buffer
    OStream<> stream(write_handler);
    std::string data = "Hello, World!";

    stream.write(StringView(data.data(), data.size()));

    // Check buffer state
    REQUIRE(stream.buffer_current() == data.size());
    REQUIRE(stream.buffer_remaining() == KTA_OSTREAM_BUFSIZE_ - data.size());

    // Check buffer contents
    const auto* buffer_data = stream.buffer_data();

    for(size_t i = 0; i < stream.buffer_current(); ++i) {
      if(i >= data.size() || i >= stream.buffer_max_size()) { KTA_PANIC("Buffer overrun??"); }
      if((char)buffer_data[i] != data[i]) { KTA_PANIC("Buffer contents mismatch"); }
    }

    stream.flush();
  }

  SECTION("WriteLargeData") {
    // Test writing data larger than the buffer
    OStream<> stream(write_handler);

    // Create a string larger than the buffer
    std::string large_data;
    large_data.reserve(KTA_OSTREAM_BUFSIZE_ + 100);
    for (size_t i = 0; i < (KTA_OSTREAM_BUFSIZE_ + 100); ++i) {
      large_data.push_back('A' + (i % 26));
    }

    stream.write(StringView(large_data.data(), large_data.size()));

    // Since the data is larger than the buffer, it should be written directly
    // and the buffer should be empty
    REQUIRE(stream.buffer_current() == 0);
    REQUIRE(stream.buffer_remaining() == KTA_OSTREAM_BUFSIZE_);
  }

  SECTION("FlushBuffer") {
    // Test flushing the buffer
    OStream<> stream(write_handler);
    std::string data = "Flush test";

    stream.write(StringView(data.data(), data.size()));
    REQUIRE(stream.buffer_current() == data.size());

    stream.flush();
    REQUIRE(stream.buffer_current() == 0);
    REQUIRE(stream.buffer_remaining() == KTA_OSTREAM_BUFSIZE_);
  }

  SECTION("BufferOverflow") {
    // Test writing data that would overflow the buffer
    OStream<> stream(write_handler);

    // Fill the buffer almost to capacity
    std::string partial_data;
    partial_data.reserve(KTA_OSTREAM_BUFSIZE_ - 4);
    for (size_t i = 0; i < (KTA_OSTREAM_BUFSIZE_ - 4); ++i) {
      partial_data.push_back('X');
    }

    stream.write(StringView(partial_data.data(), partial_data.size()));
    REQUIRE(stream.buffer_current() == partial_data.size());

    // Now write more data that would overflow
    std::string overflow_data = "Overflow";
    stream.write(StringView(overflow_data.data(), overflow_data.size()));

    // The buffer should have been flushed and the new data written
    REQUIRE(stream.buffer_current() == overflow_data.size());
    REQUIRE(stream.buffer_remaining() == KTA_OSTREAM_BUFSIZE_ - overflow_data.size());

    // Check buffer contents
    const auto* buffer_data = stream.buffer_data();

    for(size_t i = 0; i < stream.buffer_current(); ++i) {
      if(i >= overflow_data.size() || i >= stream.buffer_max_size()) { KTA_PANIC("Buffer overrun??"); }
      if((char)buffer_data[i] != overflow_data[i]) { KTA_PANIC("Buffer contents mismatch"); }
    }

    stream.flush();
  }

  SECTION("OperatorOverloads") {
    // Test operator<< overloads
    OStream<> stream(write_handler);

    // Test with different types
    // TODO: floating point types are not supported yet
    stream
      << "String: "
      << 42
      << " "
      << '\n';

    // Check buffer contents
    std::string expected = "String: 42 \n";
    const auto* buffer_data = stream.buffer_data();

    for(size_t i = 0; i < stream.buffer_current(); ++i) {
      if(i >= expected.size() || i >= stream.buffer_max_size()) { KTA_PANIC("Buffer overrun??"); }
      if((char)buffer_data[i] != expected[i]) { KTA_PANIC("Buffer contents mismatch"); }
    }

    stream.flush();
  }

  SECTION("EmptyWrite") {
    // Test writing empty data
    OStream<> stream(write_handler);

    // Write empty string
    stream.write(StringView("", 0));

    // Buffer should remain unchanged
    REQUIRE(stream.buffer_current() == 0);
    REQUIRE(stream.buffer_remaining() == KTA_OSTREAM_BUFSIZE_);
  }
}
