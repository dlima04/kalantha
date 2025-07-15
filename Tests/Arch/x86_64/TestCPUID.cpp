/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#define KTA_ASSUME_TESTING_ENV_
#include <catch2/catch_test_macros.hpp>
#include <Kalantha/Arch/x86_64/CPUID.hpp>

#include <iostream>
#include <string>

using namespace kta::x86_64;

TEST_CASE("Query Processor Feature/Version Info", "[Arch.x86_64.CPUID]") {
  CPUID obj = CPUID::get_processor_info();
  REQUIRE(obj.ecx() != 0xFFFFFFFF);
  REQUIRE(obj.edx() != 0xFFFFFFFF);

  std::cout << "---- cpuid (eax=1, ecx=0):\n";
  std::cout << std::boolalpha;

#define X(IDENT, VAL, STR) std::cout << "has " #IDENT ": " << obj.has_##IDENT() << "\n";
  CPUID_ECX_FEATURES_LIST_
  CPUID_EDX_FEATURES_LIST_
#undef X

  std::cout << std::noboolalpha;
  std::cout.flush();
}

TEST_CASE("Highest standard/extended leaves", "[Arch.x86_64.CPUID]") {
  CPUID cpuid_0(0);
  uint32_t max_basic = cpuid_0.eax();  // Highest standard leaf

  CPUID cpuid_ext(0x80000000);
  uint32_t max_extended = cpuid_ext.eax();  // Highest extended leaf

  REQUIRE(max_extended);
  REQUIRE(max_basic);

  std::cout << "Highest standard leaf: " << max_basic << "\n";
  std::cout << "Highest extended leaf: " << max_extended << std::endl;
}

TEST_CASE("Get Feature Descriptions", "[Arch.x86_64.CPUID]") {
  const std::string str1 = CPUID::get_feature_desc(CPUID::Feature::sdbg);
  REQUIRE(str1 == "Silicon Debug (IA32_DEBUG_INTERFACE MSR)");

  const std::string str2 = CPUID::get_feature_desc(CPUID::Feature::sep);
  REQUIRE(str2 == "Fast System Call");

  const std::string str3 = CPUID::get_feature_desc(CPUID::Feature::hpv);
  REQUIRE(str3 == "Hypervisor present");
}

TEST_CASE("Vendor String", "[Arch.x86_64.CPUID]") {
  CPU_Vendor vendor = CPUID::get_vendor();
  REQUIRE(vendor.buff[15] == '\0');
  if(vendor.buff[15] == '\0') {
    std::cout << "Vendor string: " << vendor.buff << std::endl;
  }
}

