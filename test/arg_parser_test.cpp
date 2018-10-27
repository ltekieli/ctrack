#include <ctrack/arg_parser.h>

#include <gtest/gtest.h>

namespace ctrack
{

template <int N, class T> inline int array_size(T (&a)[N])
{
    return N;
}

TEST(arg_parser_should, detect_missing_command_00)
{
    arg_parser ap(0, nullptr);
    std::error_code ec;
    ASSERT_THROW(ap.parse(), std::runtime_error);
}

TEST(arg_parser_should, detect_missing_command_01)
{
    arg_parser ap(1, nullptr);
    std::error_code ec;
    ASSERT_THROW(ap.parse(), std::runtime_error);
}

TEST(arg_parser_should, detect_missing_command_02)
{
    arg_parser ap(5, nullptr);
    std::error_code ec;
    ASSERT_THROW(ap.parse(), std::runtime_error);
}

TEST(arg_parser_should, parse_command)
{
    const char* args[] = {"ctrack", "cmake"};
    arg_parser ap(array_size(args), args);
    ap.parse();
    ASSERT_EQ("cmake", ap.command());
    ASSERT_EQ(0u, ap.arguments().size());
    ASSERT_EQ(0u, ap.positional_arguments().size());
}

TEST(arg_parser_should, parse_positional_arguments)
{
    const char* args[] = {"ctrack", "init", "--", "-DCMAKE_CXX_COMPILER=g++", "-GNinja"};
    arg_parser ap(array_size(args), args);
    ap.parse();
    ASSERT_EQ("init", ap.command());
    ASSERT_EQ(0u, ap.arguments().size());
    ASSERT_EQ(2u, ap.positional_arguments().size());
}

TEST(arg_parser_should, parse_parameter_arguments)
{
    const char* args[] = {"ctrack", "init", "--type", "release", "--", "-DCMAKE_CXX_COMPILER=g++", "-GNinja"};
    arg_parser ap(array_size(args), args);
    ap.parse();
    ASSERT_EQ("init", ap.command());
    ASSERT_EQ(2u, ap.arguments().size());
    ASSERT_EQ(2u, ap.positional_arguments().size());
}

TEST(arg_parser_should, tokenize_parameters)
{
    const char* args[] = {
        "ctrack", "init", "--config", "clang-ninja", "--type", "release", "--", "-DCMAKE_CXX_COMPILER=g++", "-GNinja"};
    arg_parser ap(array_size(args), args);
    ap.parse();
    ASSERT_TRUE(ap.contains("--type"));
    ASSERT_EQ("release", ap.get_value("--type"));
    ASSERT_TRUE(ap.contains("--config"));
    ASSERT_EQ("clang-ninja", ap.get_value("--config"));
}

} // namespace ctrack
