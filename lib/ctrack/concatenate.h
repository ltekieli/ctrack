#pragma once

#include <string>

namespace ctrack {

void concatenate_(std::string&) {}

template <typename T, typename ...R>
void concatenate_(std::string& result, T&& t, R&&... r) {
    result += std::string("-" + std::string(std::forward<T>(t)));
    concatenate_(result, std::forward<R>(r)...);
}

template <typename T, typename ...R>
std::string concatenate(T&& t, R&&... r) {
    std::string result(std::forward<T>(t));
    concatenate_(result, std::forward<R>(r)...);
    return result;
}

}  // namespace ctrack
