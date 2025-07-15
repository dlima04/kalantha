include(Arch)
include(Common)

if(KTA_IS_X86_64)
  add_library(tests_x86_64 OBJECT
    x86_64/TestCPUID.cpp
  )

  target_link_libraries(tests_x86_64 PUBLIC
    project_warnings
    project_options
    Catch2::Catch2WithMain
  )
endif()
