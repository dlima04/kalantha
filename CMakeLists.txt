cmake_minimum_required(VERSION 3.28)
project(runtests)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_CXX_EXTENSIONS ON)

option(USE_CLANG_ASAN "Use clang address sanitizer" OFF)
option(USE_CLANG_UBSAN "Use clang UB sanitizer" OFF)

message(STATUS "CMake -- begin init")

set(KTA_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
list(APPEND CMAKE_MODULE_PATH "${KTA_SOURCE_DIR}/CMake")

include(ClangSanitizers)
include(Arch)
include(Platform)
include(Common)

kta_enumerate_build_platform()
kta_enumerate_arch()

include_directories("${KTA_SOURCE_DIR}")
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_23)

add_library(project_warnings INTERFACE)
target_compile_options(project_warnings INTERFACE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra -Wno-pedantic>
)

if(KTA_IS_CLANGCL_WINDOWS)
  target_compile_options(project_warnings INTERFACE
    -Wno-c++98-compat-pedantic
    -Wno-reserved-macro-identifier
    -Wno-extra-semi
    -Wno-c++20-compat
  )
endif()

if(USE_CLANG_ASAN)
  kta_configure_clang_sanitizer(address)
  kta_link_clang_sanitizer(address)
endif()
if(USE_CLANG_UBSAN)
  kta_configure_clang_sanitizer(undefined)
  kta_link_clang_sanitizer(undefined)
endif()

enable_testing()
include(FetchContent)
FetchContent_Declare(
  catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.8.1
)

FetchContent_MakeAvailable(catch2)

# Add subdirectories
add_subdirectory(Tests)
add_subdirectory(Kalantha)

add_executable(runtests Tests/Entry.cpp)

# link tests
target_link_libraries(runtests PRIVATE tests_core)
target_link_libraries(runtests PRIVATE tests_allocators)

# add cmake interface libraries (headers)
target_link_libraries(runtests PRIVATE KtaCore)
target_link_libraries(runtests PRIVATE KtaMeta)
target_link_libraries(runtests PRIVATE KtaAllocators)

# Conditionally link arch-dependant libraries
if(KTA_IS_X86_64)
  target_link_libraries(runtests PRIVATE KtaX86_64)
  target_link_libraries(runtests PRIVATE tests_x86_64)
endif()

# Discover tests
include(CTest)
include(Catch)
catch_discover_tests(runtests)
