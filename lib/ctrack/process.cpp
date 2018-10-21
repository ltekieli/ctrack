#include "process.h"

#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include <stdexcept>

namespace {

char** construct_raw_argv(const std::vector<std::string>& argv) {
    char** argv_raw = new char*[argv.size() + 1];
    argv_raw[argv.size()] = nullptr;
    for (std::size_t i = 0; i < argv.size(); ++i) {
        argv_raw[i] = new char[argv[i].size() + 1];
        argv_raw[i][argv[i].size()] = '\0';
        std::copy(argv[i].begin(), argv[i].end(), argv_raw[i]);
    }
    return argv_raw;
}

}  // namespace

namespace ctrack {

int run(const std::vector<std::string>& argv) {
    auto pid = fork();
    if (pid == 0) {
        auto argv_raw = construct_raw_argv(argv);
        if (execvp("cmake", argv_raw) != 0) {
            throw std::runtime_error(strerror(errno));
        }
    }
    else if (pid == -1) {
        throw std::runtime_error(strerror(errno));
    }
    else {
        int status{};
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            return WEXITSTATUS(status);
        }
    }
    return 1;
}

}  // namespace ctrack
