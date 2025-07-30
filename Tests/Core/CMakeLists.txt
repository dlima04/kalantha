add_library(tests_core OBJECT
  TestTuple.cpp
  TestOption.cpp
  TestResult.cpp
  TestMemory.cpp
  TestArray.cpp
  TestSpan.cpp
  TestStringView.cpp
  TestAlgorithm.cpp
  TestCharConv.cpp
  TestLimits.cpp
  TestOStream.cpp
)

target_link_libraries(tests_core PUBLIC
  project_warnings
  project_options
  Catch2::Catch2WithMain
)
