include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)
add_executable(tests board_test.cpp move_generation_test.cpp)


target_link_libraries(tests
  PRIVATE
  engine
  GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(tests)
