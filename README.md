# ctrack

## USAGE

*ctrack* is a small utility which helps you track your cmake build configurations:

```
    $ cd your_cmake_project
// init clang+ninja configuration
    $ ctrack init --config clang-ninja -- -DCMAKE_CXX_COMPILER=clang++ -GNinja
// build last config
    $ ctrack build
// test last config
    $ ctrack test
// init new gcc+make configuration
    $ ctrack init --config gcc-make
// build new config
    $ ctrack build
// test new config
    $ ctrack test
// build the initial configuration
    $ ctrack build --config clang-ninja
// test clang-ninja configuration
    $ ctrack test
```

## BUILD

###### dependencies
- googletest [https://github.com/google/googletest]()
- ninja-build [https://ninja-build.org/]()
- c++17 compiler

###### gcc
```
cmake -H. -Bbuild_gcc -GNinja -DBUILD_TESTS=ON -DCMAKE_CXX_COMPILER=g++
cmake --build build_gcc
cmake --build build_gcc --target test
cmake --build build_gcc --target install
```

###### clang
```
cmake -H. -Bbuild_clang -GNinja -DBUILD_TESTS=ON -DCMAKE_CXX_COMPILER=clang++
cmake --build build_clang
cmake --build build_clang --target test
cmake --build build_clang --target install
```

## Licence
MIT
