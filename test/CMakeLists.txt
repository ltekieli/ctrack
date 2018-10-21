find_package(GTest REQUIRED)

set(TEST_LIBS
    GTest::GTest
    GTest::Main
)

add_executable(arg_parser_tests
    "arg_parser_test.cpp"
)

target_link_libraries(arg_parser_tests
  PRIVATE
    ctrack::ctrack_backend
    ${TEST_LIBS}
)

include(GoogleTest)
gtest_discover_tests(arg_parser_tests)
