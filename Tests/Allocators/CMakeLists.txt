add_library(tests_allocators OBJECT
  TestBumpAllocator.cpp
)

target_link_libraries(tests_allocators PUBLIC
  project_warnings
  project_options
  Catch2::Catch2WithMain
)
