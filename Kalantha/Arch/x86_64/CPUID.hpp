/*
* Copyright (c) 2025 Diago Lima
* SPDX-License-Identifier: BSD-3-Clause
*/

#pragma once

#include <Kalantha/Core/Common.hpp>
#include <Kalantha/Core/Types.hpp>
#include <Kalantha/Core/Platform.hpp>
BEGIN_NAMESPACE(kta::x86_64);

/*
* An extremely naive CPUID wrapper.
* lots of work needs to be done here for this to be as
* useful as other ones available.
*/

#define CPUID_ECX_FEATURES_LIST_ \
  X(sse3,    1u << 0u,  "Streaming SIMD Extensions 3") \
  X(pclmul,  1u << 1u,  "PCLMULDQ Instruction") \
  X(dtes64,  1u << 2u,  "64-Bit Debug Store") \
  X(monitor, 1u << 3u,  "MONITOR/MWAIT Instructions") \
  X(ds_cpl,  1u << 4u,  "CPL Qualified Debug Store") \
  X(vmx,     1u << 5u,  "Virtual Machine Extensions") \
  X(smx,     1u << 6u,  "Safer Mode Extensions") \
  X(est,     1u << 7u,  "Enhanced Intel SpeedStep Technology") \
  X(tm2,     1u << 8u,  "Thermal Monitor 2") \
  X(ssse3,   1u << 9u,  "Supplemental Streaming SIMD Extensions 3") \
  X(cid,     1u << 10u, "L1 Context ID") \
  X(sdbg,    1u << 11u, "Silicon Debug (IA32_DEBUG_INTERFACE MSR)") \
  X(fma,     1u << 12u, "Fused Multiply Add") \
  X(cx16,    1u << 13u, "CMPXCHG16B Instruction") \
  X(xtpr,    1u << 14u, "xTPR Update Control") \
  X(pdcm,    1u << 15u, "Perfmon and Debug Capability (IA32_PERF_CAPABILITIES MSR)") \
  X(pcid,    1u << 17u, "Process Context Identifiers") \
  X(dca,     1u << 18u, "Direct Cache Access") \
  X(sse4_1,  1u << 19u, "Streaming SIMD Extensions 4.1") \
  X(sse4_2,  1u << 20u, "Streaming SIMD Extensions 4.2") \
  X(x2apic,  1u << 21u, "Extended xAPIC Support") \
  X(movbe,   1u << 22u, "MOVBE Instruction") \
  X(popcnt,  1u << 23u, "POPCNT Instruction") \
  X(tsc_deadline, 1u << 24u, "Time Stamp Counter Deadline") \
  X(aes,     1u << 25u, "AES Instruction Extensions") \
  X(xsave,   1u << 26u, "XSAVE/XSTOR States") \
  X(osxsave, 1u << 27u, "OS-Enabled Extended State Management") \
  X(avx,     1u << 28u, "Advanced Vector Extensions") \
  X(f16c,    1u << 29u, "16-bit floating-point conversion instructions") \
  X(rdrand,  1u << 30u, "RDRAND Instruction") \
  X(hpv,     1u << 31u, "Hypervisor present")

#define CPUID_EDX_FEATURES_LIST_ \
  X(fpu,     1u << 0u,  "Floating-point Unit On-Chip") \
  X(vme,     1u << 1u,  "Virtual Mode Extension") \
  X(de,      1u << 2u,  "Debugging Extension") \
  X(pse,     1u << 3u,  "Page Size Extension") \
  X(tsc,     1u << 4u,  "Time Stamp Counter") \
  X(msr,     1u << 5u,  "Model Specific Registers") \
  X(pae,     1u << 6u,  "Physical Address Extension") \
  X(mce,     1u << 7u,  "Machine-Check Exception") \
  X(cx8,     1u << 8u,  "CMPXCHG8 Instruction") \
  X(apic,    1u << 9u,  "On-chip APIC Hardware") \
  X(sep,     1u << 11u, "Fast System Call") \
  X(mtrr,    1u << 12u, "Memory Type Range Registers") \
  X(pge,     1u << 13u, "Page Global Enable") \
  X(mca,     1u << 14u, "Machine-Check Architecture") \
  X(cmov,    1u << 15u, "Conditional Move Instruction") \
  X(pat,     1u << 16u, "Page Attribute Table") \
  X(pse36,   1u << 17u, "36-bit Page Size Extension") \
  X(psn,     1u << 18u, "Processor serial number is present and enabled") \
  X(clflush, 1u << 19u, "CLFLUSH Instruction") \
  X(ds,      1u << 21u, "DS") \
  X(acpi,    1u << 22u, "ACPI") \
  X(mmx,     1u << 23u, "MMX") \
  X(fxsr,    1u << 24u, "FXSR") \
  X(sse,     1u << 25u, "Streaming SIMD Extensions") \
  X(sse2,    1u << 26u, "Streaming SIMD Extensions 2") \
  X(ss,      1u << 27u, "Self-Snoop") \
  X(htt,     1u << 28u, "Multi-Threading") \
  X(tm,      1u << 29u, "Thermal Monitor") \
  X(ia64,    1u << 30u, "IA64 processor emulating x86") \
  X(pbe,     1u << 31u, "Pending Break Enable")

struct CPU_Vendor {
  char buff[16]{};
};

class CPUID {
public:
  using RegType = uint32;
  explicit CPUID(RegType func, RegType in_ecx = 0);

  RegType eax() const { return eax_; }
  RegType ebx() const { return ebx_; }
  RegType ecx() const { return ecx_; }
  RegType edx() const { return edx_; }

  enum class Feature : RegType {
  #define X(IDENT, VALUE, UNUSED) IDENT,
    CPUID_ECX_FEATURES_LIST_
    CPUID_EDX_FEATURES_LIST_
  #undef X
  };

  #define X(IDENT, VALUE, UNUSED) bool has_##IDENT() const { return ecx_ & (VALUE); }
    CPUID_ECX_FEATURES_LIST_
  #undef X

  #define X(IDENT, VALUE, UNUSED) bool has_##IDENT() const { return edx_ & (VALUE); }
    CPUID_EDX_FEATURES_LIST_
  #undef X

  bool has_feature(const CPUID::Feature feature) const {
    switch (feature) {
  #define X(IDENT, VALUE, UNUSED) case Feature::IDENT: return has_##IDENT();
    CPUID_ECX_FEATURES_LIST_
    CPUID_EDX_FEATURES_LIST_
  #undef X
    default: return false;
    }
  }

  static const char* get_feature_desc(const CPUID::Feature feature) {
    switch(feature) {
    #define X(IDENT, VALUE, STR) case CPUID::Feature::IDENT: return STR;
      CPUID_ECX_FEATURES_LIST_
      CPUID_EDX_FEATURES_LIST_
    #undef X
    default: break;
    }
    return "???";
  }

  static CPU_Vendor get_vendor() {
    RegType regs[5]{};

    asm volatile("cpuid"
      : "=a"(regs[0]), "=b"(regs[1]), "=c"(regs[2]), "=d"(regs[3])
      : "a"(0));

    CPU_Vendor vendor{};
    __builtin_memcpy(&vendor.buff[0], &regs[1], sizeof(RegType));
    __builtin_memcpy(&vendor.buff[4], &regs[3], sizeof(RegType));
    __builtin_memcpy(&vendor.buff[8], &regs[2], sizeof(RegType));

    vendor.buff[15] = '\0';
    return vendor;
  }

  static const char* feature_to_string(const CPUID::Feature feature) {
    switch(feature) {
    #define X(IDENT, VALUE, UNUSED) case CPUID::Feature::IDENT: return #IDENT;
      CPUID_ECX_FEATURES_LIST_
      CPUID_EDX_FEATURES_LIST_
    #undef X
    default: break;
    }
    return "???";
  }

  static CPUID get_processor_info() { return CPUID(1); }

  CPUID(const CPUID&) = default;
  CPUID& operator=(const CPUID&) = default;

  CPUID(CPUID&&) = default;
  CPUID& operator=(CPUID&&) = default;
private:
  volatile RegType eax_ = 0xFFFFFFFF;
  volatile RegType ebx_ = 0xFFFFFFFF;
  volatile RegType ecx_ = 0xFFFFFFFF;
  volatile RegType edx_ = 0xFFFFFFFF;
};

inline CPUID::CPUID(RegType func, RegType in_ecx) {
  asm volatile("cpuid"
    : "=a"(eax_), "=b"(ebx_), "=c"(ecx_), "=d"(edx_)
    : "a"(func), "c"(in_ecx));
}

END_NAMESPACE(kta::x86_64);
