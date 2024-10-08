# myxml

![Static Badge](https://img.shields.io/badge/C%2B%2B-17-blue) ![GitHub License](https://img.shields.io/github/license/Adamska1008/myxml) ![GitHub Actions Workflow Status](https://img.shields.io/github/actions/workflow/status/Adamska1008/myxml/cmake-single-platform.yml) [![codecov](https://codecov.io/github/Adamska1008/myxml/graph/badge.svg?token=W1PZ6EZ17M)](https://codecov.io/github/Adamska1008/myxml)

>Extensible Markup Language (XML) is a markup language and file format for storing, transmitting, and reconstructing arbitrary data. It defines a set of rules for encoding documents in a format that is both human-readable and machine-readable. The World Wide Web Consortium's XML 1.0 Specification of 1998 and several other related specifications—all of them free open standards—define XML.
>
> [Learn more about XML on Wikipedia](https://en.wikipedia.org/wiki/XML)

**myxml** is a minimal and user-friendly XML DOM parser designed to be easy to integrate and extend.

## Why myxml?

**myxml** is developed in modern C++(17), leveraging the capabilities of `string_view`, `shared_ptr`, `optional`, `variant` and some of newest SNIFAE utility(sadly not concept). It's fully modulized(not all in one file) and has a well-designed Is-a relationship.

Many of the most-starred XML parsers on GitHub are somewhat outdated and not as handy as popular JSON parsers like [nlohmann](https://github.com/nlohmann/json). It’s time to create a modern alternative.

Currently **myxml** has only around 2000 lines of code, making it easy to get involved in its development. While it's far far from perfect, myxml is perfect for anyone looking to contribute to a modern C++ project. Share your ideas with us!

## Progress and Roadmap

The repo is still a work in progress and has not yet fully support all XML features. Contributions are welcome! Read [Contribution Guide](./docs/contribution_guide.md) for more information. Turn to [issues](https://github.com/Adamska1008/myxml/issues) to see what I think we should do.

### What it can do now

- Parsing nested elements with text and attributes
- Entity encoding and newline normalization in text
- Parsing document with declaration and a root element
- Parsing CDATA
- Providing parsing error info

### TODOs

- Support full-featured Processing Instructions
- Implement XML Namespace Parsing
- Add XPath Support
- Integrate DTD/XML Schema Validation
- Refactor for Zero-Copy Efficiency

### Zero-Copy?

**myxml** uses `XMLFile` class to load files, leveraging `mmap` to opening files. Additionally **myxml** implements `StringBuffer` class, which holds a reference to any string used to build the DOM tree, or stores a moved string object. While **myxml** doesn't copy the entire data of an XMLDocument, it does copy Text and CDATA instead of referencing the source data. This is based on the assumption that user may drop raw string or close file after parsing, which may lead to dangling reference. Maybe **myxml** can offer an option for users who explicitly require zero-copy implementation in the future.

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

Here’s a **basic example**:

```c++
#include <myxml/element.hpp>

int main()
{
    auto elem = myxml::element::parse("<root attr=\"value\">Hello, world!<root/>");
    std::cout << elem.first_text() << std::endl;    // "Hello, world!";
    std::cout << elem["attr"] << std::endl;      // "value"
}
```

**Create Element using custom string literal**:

```C++
#include <myxml/element.hpp>
// enable literals by using namespace
using namespace xml::literals;

int main()
{
    auto elem = "<root></root>"_elem;
    std::cout << elem.name() << std::endl; // root
}
```

Similar Approach for `myxml::document`.

**Parse file**:

```C++
#include <myxml/document.hpp>

int main()
{
    auto doc = myxml::document::load("filename");
    auto root = doc.root();
}
```

For more examples, read [examples](./docs/examples.md) and test files [here](./tests/).

## Code Coverage

**myxml** support code coverage analysis via [Codecov](https://about.codecov.io/). Install Codecov browser extension to visualize which lines of code are covered and which are not.

## Inspiration

This repo is inspired by [tinyxml2](https://github.com/leethomason/tinyxml2), [pugixml](https://github.com/zeux/pugixml) and [nlohmann/json](https://github.com/nlohmann/json).
