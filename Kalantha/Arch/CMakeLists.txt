add_library(KtaX86_64 INTERFACE
  x86_64/CPUID.hpp
)

add_library(KtaGenericArch INTERFACE
  Generic/WhichArch.hpp
  Generic/Endian.hpp
)
