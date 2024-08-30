# myxml

>Extensible Markup Language (XML) is a markup language and file format for storing, transmitting, and reconstructing arbitrary data. It defines a set of rules for encoding documents in a format that is both human-readable and machine-readable. The World Wide Web Consortium's XML 1.0 Specification of 1998 and several other related specifications—all of them free open standards—define XML.
>
> [Learn more about XML on Wikipedia](https://en.wikipedia.org/wiki/XML)

**myxml** is a simple, stable, and user-friendly XML DOM parser designed to be easy to integrate and extend.

## Features

The repo is still a work in progress and has not yet fully support XML features. Contributions are welcome! Read [Contribution Guide](./docs/contribution_guide.md) for more information. Turn to [issues](https://github.com/Adamska1008/myxml/issues) to see what we aims to accomplish next.

### What it can do now

- Parsing nested elements with text and attributes
- Entity encoding and newline normalization in text
- Parsing document with declaration and a root element
- Parsing CDATA
- Providing parsing error info

### TODOs

- Implement XML Namespace Parsing
- Add XPath Support
- Integrate DTD/XML Schema Validation
- Refactor for Zero-Copy Efficiency

## Setup

### Dependencies

**myxml** uses git submodule to manage dependencies. Run following commands first:

```bash
git submodule init
git submodule update
```

### Compilation

Ensure that your environment meets the following requirements:

- CMake version > 3.22
- C++ compiler with C++17 support

To compile the project, navigate to the project directory and run:

```bash
cmake -S . -B build
cmake --build build
```

If you don't want to compile testing, add `-DBUILD_TESING=OFF` to the configure command.

### Running tests

In the project directory, run:

```bash
ctest --test-dir build/tests/
```

### Installation

In the project directory, run:

```bash
cmake --install build # usually requires root permission
```

Then your C++ Compiler will be able to find **myxml** directly.

### Integration

If you want to integrate **myxml** into your CMake project, add this repo to your project and modify your root `CMakeLists.txt`:

```cmake
add_subdirectory(myxml)  # or `deps/myxml`, as you like
```

Linking myxml to any target that requires it:

```cmake
target_link_libraries(your_target PRIVATE myxml)
```

## Usage

Here’s a basic example:

```c++
#include <element.hpp>

int main()
{
    std::string xml("<root attr=\"value\">Hello, world!<root/>");
    auto elem = myxml::Element::Parse(xml);
}
```

For more examples, read test files [here](./tests/).

## Why not tinyxml2

**tinyxml2** aims to work in all conditions, so it abandons STL, RTTI or exceptions. Such design rule may cause several issues like a level of instability, for example [issue932](https://github.com/leethomason/tinyxml2/issues/932).

What's more, **tinyxml2** is too old to be maintained or enhanced with new features. In contrast, **myxml** offers a more modern, readable codebase that is easier to extend and maintain.

## Inspiration

This repo is inspired by [tinyxml2](https://github.com/leethomason/tinyxml2), [pugixml](https://github.com/zeux/pugixml) and the [serde](https://crates.io/crates/serde) serialization framework.
