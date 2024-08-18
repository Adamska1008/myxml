# myxml

**myxml** is a xml dom parser which aims to be simple, stable and easy to use. 

## Features

- **Super effcient**
- **Easy-to-use Api**
- **Text Lazy-Loading**
- **Rich Test Cases**

## Why not tinyxml2

**tinyxml2** implements a useful xml parser. However, it aims to provide functionality in all conditions and abandons STL, RTTI or exceptions. Such design rule may cause several issues. It causes level of instability, for example [issue932](https://github.com/leethomason/tinyxml2/issues/932). It implements attributes with linked list, while hashmap may be much more faster. With the power of STL, **myxml** can get element by name and get attribute in exponential time complexity.

## Example



## Inspiration

This repo is inspired by [tinyxml2](https://github.com/leethomason/tinyxml2) and [serde](https://crates.io/crates/serde).