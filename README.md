<a href="https://sqids.org/cpp">
  <h1 align="center">Sqids C++</h1>
</a>

<p align="center">
  <a href="https://github.com/sqids/sqids-cpp/actions/workflows/cmake.yml">
    <img alt="" src="https://github.com/sqids/sqids-cpp/actions/workflows/cmake.yml/badge.svg" />
  </a>
  <a href="https://opensource.org/licenses/MIT">
    <img alt="" src="https://img.shields.io/badge/license-MIT-yellow.svg" />
  </a>
  <a href="https://isocpp.org">
    <img alt="" src="https://img.shields.io/badge/language-C%2B%2B11-blue.svg" />
  </a>
  <a href="https://cmake.org/">
    <img alt="" src="https://img.shields.io/badge/build-CMake-orange" />
  </a>
</p>

<p align="center">
  Sqids (<em>pronounced "squids"</em>) is a small library that lets you generate YouTube-looking IDs from numbers. It's good for link shortening, fast & URL-safe ID generation and decoding back into numbers for quicker database lookups.
</p>

## Getting started

@todo

## Examples

@todo

## Installing

```hash
mkdir -p build
cd build
cmake ..
cmake --build . --config Release --target install -- -j $(nproc)
```

The last command may need to run as `sudo`.

## Using the library

A minimal `CMakeLists.txt` configuration:

```cmake
project("sqids-example")

find_package(sqids CONFIG REQUIRED)

add_executable(sqids-example main.cpp)
target_link_libraries(sqids-example INTERFACE sqids)
```

```cpp
#include <iostream>
#include <sqids/sqids.hpp>

int main() 
{
    sqidscxx::Sqids sqids;
    std::cout << sqids.encode({ 1, 2, 3 }) << std::endl;
    return 0;
}
```

## Running the tests

```bash
cmake -S . -B build 
cmake --build build
cd build 
ctest
```

## License

[MIT](LICENSE)
