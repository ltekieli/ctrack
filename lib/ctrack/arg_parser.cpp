#include "arg_parser.h"

#include <algorithm>
#include <queue>

namespace ctrack {

namespace {

constexpr auto delimiter = "--";
const std::string prefix = std::string{"--"};

}  // namespace

arg_parser::arg_parser(int argc, const char* argv[])
{
    if (argv) {
        m_arguments = std::vector<std::string>{argv, argv + argc};
    }
}

void arg_parser::parse() {
    if (m_arguments.size() < 2) {
        throw std::runtime_error("Command not specified");
    }
    parse_command();
    parse_parameters();
    tokenize();
}

bool arg_parser::contains(const parameter& p) {
    return 0 != m_tokens.count(p);
}

const arg_parser::value& arg_parser::get_value(const parameter& p) {
    return get_values(p)[0];
}

const arg_parser::values& arg_parser::get_values(const parameter& p) {
    if (0 == m_tokens.count(p)) {
        throw std::runtime_error("Argument not specified");
    }
    const auto& vs = m_tokens[p];
    if (0 == vs.size()) {
        throw std::runtime_error("Value for argument not specified");
    }
    return vs;
}

const std::string& arg_parser::command() const {
    return m_command;
}

const std::vector<std::string>& arg_parser::arguments() const {
    return m_parameters;
}

const std::vector<std::string>& arg_parser::positional_arguments() const {
    return m_positional;
}

void arg_parser::parse_command() {
    m_command = m_arguments[1];
}

void arg_parser::parse_parameters() {
    auto begin = m_arguments.begin();
    std::advance(begin, 2);
    if (begin != m_arguments.end()) {
        auto iter = std::find(begin, m_arguments.end(), delimiter);
        m_parameters = std::vector<std::string>(begin, iter);
        if (iter != m_arguments.end()) {
            ++iter;
            m_positional = std::vector<std::string>(iter, m_arguments.end());
        }
    }
}

void arg_parser::tokenize() {
    std::queue<std::string> q{std::deque<std::string>(m_arguments.begin(), m_arguments.end())};
    auto last = m_tokens.end();
    while (!q.empty()) {
        auto token = q.front();
        q.pop();
        if (token.compare(0, prefix.size(), prefix) == 0) {
            std::tie(last, std::ignore) = m_tokens.insert({token, {}});
        } else if (last != m_tokens.end()) {
            last->second.push_back(token);
        }
    }
}

}  // namespace ctrack
