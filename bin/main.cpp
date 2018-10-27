#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <ctrack/arg_parser.h>
#include <ctrack/concatenate.h>
#include <ctrack/process.h>

/*
 * ctrack init
 * ctrack init --config {name} -- -DCMAKE_CXX_COMPILER=clang++ -GNinja
 * ctrack build
 * ctrack build --config {name}
 * ctrack test
 * ctrack test --config {name}
 * ctrack clean
 * ctrack clean --config {name}
 * ctrack install
 * ctrack install --config {name}
 */

constexpr auto default_config = "default";
constexpr auto ctrack_file_name = ".ctrack";
constexpr auto param_config = "--config";

constexpr auto help_message = R"(
Usage: ctrack <command> <options> -- <cmake options>

Available commands:
  init      Initializes cmake build directory
  build     Executes build
  test      Executes tests
  clean     Cleans build directory
  install   Executes install
  help      Prints this help message

Common options:
  --config <name>       Configuration to init or build/test/clean/install
)";

std::string get_project_name()
{
    return std::filesystem::current_path().filename().string();
}

std::filesystem::path get_build_path(const std::string& project_name, const std::string& config)
{
    if (std::filesystem::current_path() != std::filesystem::path("/"))
    {
        auto build_dir = ctrack::concatenate(project_name, "build", config);
        return std::filesystem::current_path().parent_path().append(build_dir);
    }
    throw std::runtime_error("Called from root directory");
}

std::string get_config_name(ctrack::arg_parser& ap)
{
    std::string config(default_config);
    if (ap.contains(param_config))
    {
        config = ap.get_value(param_config);
        std::fstream config_file(ctrack_file_name, std::fstream::out);
        config_file << config;
    }
    else
    {
        std::ifstream config_file(ctrack_file_name);
        if (config_file.good())
            config_file >> config;
    }
    return config;
}

int handle_init(ctrack::arg_parser& ap)
{
    auto config = get_config_name(ap);
    auto project_name = get_project_name();
    auto build_path = get_build_path(project_name, config);
    std::vector<std::string> argv = {
        "cmake",
        "-H.",
        "-B" + build_path.string(),
    };
    const auto& args_pos = ap.positional_arguments();
    std::copy(args_pos.begin(), args_pos.end(), std::back_inserter(argv));
    return ctrack::run(argv);
}

int handle_build_target(ctrack::arg_parser& ap, std::string target)
{
    auto config = get_config_name(ap);
    auto project_name = get_project_name();
    auto build_path = get_build_path(project_name, config);

    if (!std::filesystem::exists(build_path))
    {
        throw std::runtime_error("Given config does not exist");
    }

    std::vector<std::string> argv = {"cmake", "--build", build_path.string(), "--target", target};
    const auto& args_pos = ap.positional_arguments();
    std::copy(args_pos.begin(), args_pos.end(), std::back_inserter(argv));
    return ctrack::run(argv);
}

int main(int argc, const char* argv[])
{
    try
    {
        ctrack::arg_parser ap(argc, argv);
        ap.parse();
        auto command = ap.command();
        if ("init" == command)
        {
            handle_init(ap);
        }
        else if ("build" == command)
        {
            return handle_build_target(ap, "all");
        }
        else if ("test" == command)
        {
            auto result = handle_build_target(ap, "all");
            if (result != 0)
            {
                return result;
            }
            return handle_build_target(ap, "test");
        }
        else if ("clean" == command)
        {
            return handle_build_target(ap, "clean");
        }
        else if ("install" == command)
        {
            return handle_build_target(ap, "install");
        }
        else if ("help" == command)
        {
            std::cout << help_message << std::endl;
        }
        else
        {
            std::cerr << "ERROR: Unrecognized command: " << command << std::endl;
            std::cerr << help_message << std::endl;
            return 1;
        }
    }
    catch (std::runtime_error& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        std::cerr << help_message << std::endl;
        return 1;
    }
    return 0;
}
