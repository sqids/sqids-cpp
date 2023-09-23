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

Features:

- **Encode multiple numbers** - generate short IDs from one or several non-negative numbers
- **Quick decoding** - easily decode IDs back into numbers
- **Unique IDs** - generate unique IDs by shuffling the alphabet once
- **ID padding** - provide minimum length to make IDs more uniform
- **URL safe** - auto-generated IDs do not contain common profanity
- **Randomized output** - Sequential input provides nonconsecutive IDs
- **Many implementations** - Support for [40+ programming languages](https://sqids.org/)
- **Header-only library** - Simply drop the `include` directory into your project root

## 🧰 Use-cases

Good for:

- Generating IDs for public URLs (eg: link shortening)
- Generating IDs for internal systems (eg: event tracking)
- Decoding for quicker database lookups (eg: by primary keys)

Not good for:

- Sensitive data (this is not an encryption library)
- User IDs (can be decoded revealing user count)

## 🚀 Getting started

### Installing

> **Note**
> 🚧 This step is fully optional. The easiest way to use this library is to merge the `include` directory into your project root and then directly include the `sqids.hpp` header:
> ```cpp
> #include "include/sqids/sqids.hpp"
> ```

To install the headers on your system using CMake, run:

```bash
mkdir -p build
cd build
cmake ..
cmake --build . --config Release --target install
```

The last command typically needs to run as `sudo`.

(By default, the headers will be installed to `include/sqids/`, relative to the CMake install prefix, which is usually `/usr/local/` on Linux/Unix.) 

#### Including the library

In your code, include the library with:

```cpp
#include <sqids/sqids.hpp>
```

For CMake-based projects, a minimal `CMakeLists.txt` configuration may then consist of the following:

```cmake
project("sqids-example")

find_package(sqids CONFIG REQUIRED)

add_executable(sqids-example main.cpp)
target_link_libraries(sqids-example INTERFACE sqids)
```

### Running the tests

```bash
cmake -S . -B build 
cmake --build build
cd build 
ctest -V
```

## 👩‍💻 Examples

### Encoding:

@todo

### Decoding:

@todo

## 📝 License

[MIT](LICENSE)
