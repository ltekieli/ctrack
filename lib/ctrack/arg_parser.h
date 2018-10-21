#pragma once

#include <map>
#include <string>
#include <system_error>
#include <vector>

#include "error.h"

namespace ctrack {

class arg_parser {
public:
    using parameter = std::string;
    using value = std::string;
    using values = std::vector<value>;

    arg_parser(int, const char*[]);

    void parse();
    bool contains(const parameter&);
    const value& get_value(const parameter&);
    const values& get_values(const parameter&);

    const std::string& command() const;
    const std::vector<std::string>& arguments() const;
    const std::vector<std::string>& positional_arguments() const;

private:
    void parse_command();
    void parse_parameters();
    void tokenize();

    std::vector<std::string> m_arguments;
    std::string m_command;
    std::vector<std::string> m_parameters;
    std::vector<std::string> m_positional;

    std::map<parameter, values> m_tokens;
};

}  // namespace ctrack
