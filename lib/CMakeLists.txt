add_library(ctrack_backend
    ctrack/arg_parser.cpp
    ctrack/process.cpp
)

target_compile_features(ctrack_backend INTERFACE cxx_std_17)

target_include_directories(ctrack_backend
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

add_library(ctrack::ctrack_backend ALIAS ctrack_backend)
