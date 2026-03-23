add_executable(test_am2302_tools
  test_am2302_tools.cpp
  ../src/AM2302-Sensor_Tools.cpp
)

target_include_directories(test_am2302_tools PRIVATE
  ../src
)

target_link_libraries(test_am2302_tools
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_am2302_tools)
